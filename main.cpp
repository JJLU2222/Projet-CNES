// compilation : g++ -std=c++20 main.cpp -o main_exe $(pkg-config --cflags --libs libgpiod)

#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <random>
#include <gpiod.h>
#include "sensor.h"
#include <fcntl.h>    // For O_* constants
#include <sys/mman.h> // For shared memory
#include <sys/stat.h> // For mode constants

// Semaphore initialization: max value = 0 (thread2 is blocked initially)
std::counting_semaphore<1> sem_sensor(0); // A semaphore with initial count of 0
std::counting_semaphore<1> sem_sensor_ready(0);
std::counting_semaphore<1> sem_vanne(0); // A semaphore with initial count of 0

#define SHM_Sensor "/sensor_shm"
#define SHM_Vanne "/vanne_shm"

#define NCapteur 8
#define NVanne 1

// Config GPIO
#define CHIP_PATH "/dev/gpiochip0"
#define GPIO_PIN 5

void init_gpio();
gpiod_chip *chip;
gpiod_line lines;

struct SensorData
{
    double return_data[NCapteur];
};

struct EtatVanne
{
    bool etat[NVanne];
};

void process_sensor()
{

    // Creer SHM (data of sensor)
    int shm_fd_sensor = shm_open(SHM_Sensor, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_sensor, sizeof(SensorData));
    SensorData *data = (SensorData *)mmap(0, sizeof(SensorData), PROT_WRITE, MAP_SHARED, shm_fd_sensor, 0);

    while (true)
    {
        sem_sensor.acquire(); // Wait for the semaphore signal

        readChannels();
        for (int i = 0; i < NCapteur; i++)
        {
            data->return_data[i] = valSensors[i];
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
    EtatVanne *etat_vanne = (EtatVanne *)mmap(0, sizeof(EtatVanne), PROT_READ, MAP_SHARED, shm_fd_vanne, 0);

    init_gpio();

    while (true)
    {
        sem_vanne.acquire(); // Wait for the semaphore signal

        for (int i = 0; i < NVanne; ++i)
        {
            gpiod_line_set_value(lines, etat_vanne->etat[i]);
        }
    }

    for (int i = 0; i < NVanne; ++i)
    {
        gpiod_line_release(lines);
    }
    gpiod_chip_close(chip);
}

void process_communication()
{
    char userInput;

    // Creer SHM vanne
    int shm_fd_vanne = shm_open(SHM_Vanne, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_vanne, sizeof(EtatVanne));
    EtatVanne *etat_vanne = (EtatVanne *)mmap(0, sizeof(EtatVanne), PROT_WRITE, MAP_SHARED, shm_fd_vanne, 0);

    // Ouvrir SHM Sensor
    int shm_fd_sensor = shm_open(SHM_Sensor, O_CREAT | O_RDWR, 0666);
    if (shm_fd_sensor == -1)
    {
        std::cerr << "Failed to open shared memory!" << std::endl;
        exit(EXIT_FAILURE);
    }

    SensorData *data = (SensorData *)mmap(0, sizeof(SensorData), PROT_READ, MAP_SHARED, shm_fd_sensor, 0);

    while (true)
    {
        std::cout << "Enter 'S' to trigger Process sensor, 'L' to trigger Process led : ";
        std::cin >> userInput;

        if (userInput == 'S')
        {
            sem_sensor.release(); // Release the semaphore to allow process 2 to run

            sem_sensor_ready.acquire();
            std::cout << "Data of sensor receive : ";
            for (int i = 0; i < NCapteur; ++i)
            {
                std::cout << data->return_data[i] << " ";
            }
            std::cout << std::endl;
        }
        else if (userInput == 'L')
        {
            etat_vanne->etat[0] = !etat_vanne->etat[0];

            sem_vanne.release(); // Release the semaphore to allow process 2 to run
        }
    }
}

int main()
{
    // Create threads
    std::thread t1(process_sensor);
    std::thread t2(process_vanne);
    std::thread t3(process_communication);

    // Wait for threads to finish
    t1.join();
    t2.join();

    return 0;
}

void init_gpio()
{
    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        std::cerr << "Failed to open GPIO chip!" << std::endl;
        exit(EXIT_FAILURE);
    }

    gpiod_line *line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!line)
    {
        std::cerr << "Failed to get line!" << std::endl;
        gpiod_chip_close(chip);
        exit(EXIT_FAILURE);
    }
    int ret = gpiod_line_request_output(line, "led_control", 0);
    if (ret < 0)
    {
        std::cerr << "Failed to request line as output!" << std::endl;
        gpiod_chip_close(chip);
        exit(EXIT_FAILURE);
    }
    lines[0] = line;
}