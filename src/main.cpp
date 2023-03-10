#include <sched.h>
#include <unistd.h>
#include <string>
#include <csignal>
#include <sys/wait.h>
#include <cstdio>


static char childStack[4096];

const char* const childArgs[] = {"/bin/bash", "-c", "mount --make-private -t proc proc /proc && /bin/bash", nullptr};


int child_main(void* args){
    printf("launch child process\n");
    std::string newHostName = "winlic-d1.muc";
    sethostname(newHostName.data(), newHostName.size());
    execv(childArgs[0], const_cast<char *const*>(&childArgs[0]));
    return 0;
}

int main(){
    constexpr int flag = CLONE_NEWNS | CLONE_NEWPID| CLONE_NEWIPC|CLONE_NEWUTS | SIGCHLD;
    pid_t child = clone(child_main, &childStack[sizeof(childStack)],flag, nullptr);
    if(child>=0)
    {
        waitpid(child, nullptr, 0);
        printf("exit child process\n");
    }else{
        perror("clone failed");
    }
    return 0;
}