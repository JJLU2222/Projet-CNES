/* compilation :
g++ -std=c++20 main.cpp valve.cpp sensor.cpp -o main_exe $(pkg-config --cflags --libs libgpiod)
*/
#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <random>
#include <gpiod.h>
#include "sensor.h"
#include "valve.h"
#include "configCAC.h"
#include <fcntl.h>    // For O_* constants
#include <sys/mman.h> // For shared memory
#include <sys/stat.h> // For mode constants

// Semaphore initialization: max value = 0 (thread2 is blocked initially)
std::counting_semaphore<1> sem_sensor(0); // A semaphore with initial count of 0
std::counting_semaphore<1> sem_sensor_ready(0);
std::counting_semaphore<1> sem_vanne(0); // A semaphore with initial count of 0

#define SHM_Sensor "/sensor_shm"
#define SHM_Vanne "/vanne_shm"

#define NCapteur 4
#define NVanne 4

struct SensorData
{
    Sensor sensors[NCapteur];
};

struct VanneData
{
    Valve vannes[NVanne];
};

void process_sensor()
{
    // Ouvrir SHM Sensor
    int shm_fd_sensor = shm_open(SHM_Sensor, O_CREAT | O_RDWR, 0666);
    if (shm_fd_sensor == -1)
    {
        std::cerr << "Failed to open shared memory!" << std::endl;
        exit(EXIT_FAILURE);
    }
    SensorData *espace_sensors = (SensorData *)mmap(0, sizeof(SensorData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd_sensor, 0);

    while (true)
    {
        sem_sensor.acquire(); // Wait for the semaphore signal

        for (int i = 0; i < NCapteur; i++)
        {
            if (espace_sensors->sensors[i].readChannel() != noError)
            {
                std::cerr << "Erreur lors de la lecture du canal." << std::endl;
            }
        }

        sem_sensor_ready.release();
    }
}

void process_vanne()
{
    // Ouvrir SHM Vanne
    int shm_fd_vanne = shm_open(SHM_Vanne, O_CREAT | O_RDWR, 0666);
    if (shm_fd_vanne == -1)
    {
        std::cerr << "Failed to open shared memory!" << std::endl;
        exit(EXIT_FAILURE);
    }
    VanneData *espace_vannes = (VanneData *)mmap(0, sizeof(VanneData), PROT_READ, MAP_SHARED, shm_fd_vanne, 0);

    while (true)
    {
        sem_vanne.acquire(); // Wait for the semaphore signal

        for (int i = 0; i < NVanne; ++i)
        {
            espace_vannes->vannes[i].apply_change();
        }
    }
}

int main()
{
    // ———————————————————————— Init vanne ————————————————————————————————————————————————
    //  Creer SHM vanne
    int shm_fd_vanne = shm_open(SHM_Vanne, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_vanne, sizeof(VanneData));
    VanneData *espace_vannes = (VanneData *)mmap(0, sizeof(VanneData), PROT_WRITE, MAP_SHARED, shm_fd_vanne, 0);

    // Création des objets Valve
    for (int i = 0; i < NVanne; ++i)
    {
        new (&espace_vannes->vannes[i]) Valve(CACMO_Vanne_name[i], CAC_Name[0], CACMO_Vanne_pin[i]);
    }

    // Initialisation
    for (int i = 0; i < NVanne; ++i)
    {
        if (espace_vannes->vannes[i].init() != noError)
        {
            std::cerr << "Erreur d'initialisation des GPIO" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // ———————————————————————— Init capteur ————————————————————————————————————————————————
    //  Creer SHM (data of sensor)
    int shm_fd_sensor = shm_open(SHM_Sensor, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_sensor, sizeof(SensorData));
    SensorData *espace_sensors = (SensorData *)mmap(0, sizeof(SensorData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd_sensor, 0);

    // Création des objets sensor
    for (int i = 0; i < NCapteur; ++i)
    {
        new (&espace_sensors->sensors[i]) Sensor(CACMO_Sensor_name[i], CAC_Name[0], 1, CACMO_Sensor_channel[i]);
    }

    // Initialisation
    for (int i = 0; i < NCapteur; ++i)
    {
        if (espace_sensors->sensors[i].initSensor() != noError)
        {
            std::cerr << "Erreur d'initialisation des Sensor" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Create threads
    std::thread t1(process_sensor);
    std::thread t2(process_vanne);

    char userInput;
    while (true)
    {
        std::cout << "Enter 'S' to trigger Process sensor, 'L' to trigger Process led : ";
        std::cin >> userInput;

        if (userInput == 'S')
        {
            sem_sensor.release(); // Release the semaphore to allow process 2 to run

            sem_sensor_ready.acquire();
            std::cout << "Data of sensor receive : ";
            // Affichage de la valeur lue
            for (int i = 0; i < NCapteur; ++i)
            {
                espace_sensors->sensors[i].print_value();
            }
        }
        else if (userInput == 'L')
        {
            for (int i = 0; i < NVanne; ++i)
            {
                int etat = espace_vannes->vannes[i].state;
                espace_vannes->vannes[i].state = 1 - etat;
            }

            sem_vanne.release(); // Release the semaphore to allow process 2 to run
        }
    }

    // Wait for threads to finish
    t1.join();
    t2.join();
    return 0;
}
