#include "SharedMemoryController.h"

namespace driver::common {

SharedMemoryController::SharedMemoryController() : mThread(std::bind(&SharedMemoryController::execute, this))
{
    mThread.detach();
}

void SharedMemoryController::execute()
{
    int shmid;
    void *shared_memory;
    struct Person person;

    // Tạo vùng nhớ chia sẻ với key 2345 và kích thước phù hợp với struct Person
    shmid = shmget((key_t)2345, sizeof(struct Person), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Gắn process vào vùng nhớ chia sẻ
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("Key of shared memory is %d\n", shmid);
    printf("Process attached at %p\n", shared_memory);

    // Nhập dữ liệu từ người dùng
    // printf("Enter name: ");
    // fgets(person.name, sizeof(person.name), stdin);
    // person.name[strcspn(person.name, "\n")] = 0;  // Xóa ký tự newline
    // printf("Enter age: ");
    // scanf("%d", &person.age);
    // printf("Enter phone number: ");
    // scanf("%s", person.phone);

    int i = 0;
    while(true)
    {

        person.name = "bao";
        person.age = ++i;
        person.phone = "0935008291";
        // Ghi dữ liệu struct vào shared memory
        memcpy(shared_memory, &person, sizeof(struct Person));

        // std::cout << "Receive age: " << person.age << std::endl;
        sleep(1);
    }

}

}