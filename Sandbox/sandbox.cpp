#define _POSIX_C_SOURCE 200112L

#include "sys_call_table.h"

/* C standard library */
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* POSIX */
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>

/* Linux */
#include <syscall.h>
#include <sys/ptrace.h>

#include <bits/stdc++.h>
#include "parser.h"
#include "automata.h"
//using namespace std;

#define FATAL(...) \
    do { \
        fprintf(stderr, "strace: " __VA_ARGS__); \
        fputc('\n', stderr); \
        exit(EXIT_FAILURE); \
    } while (0)

void displayAddress(set<long int> &addr){
    int count=0;
    for(auto x : addr){
        if(count%6==0){
            cout<<endl;
        }
        count++;
        printf("0x%lx\t", x);
    }
    cout<<endl<<endl;
}
    
bool isMovePossible(string input, pid_t pid, struct user_regs_struct regs){

    set<int> addresses=setOfPossibleAddressOnInput(input);
    if (addresses.size()==0){
        if(input=="exit_group"){
            std::cout<<"SUCCESS!!"<<std::endl;
            return true;
        }
        else{
        cout<<"Automata says that the above system call cannot be made from any viable address!!"<<endl;
        return false;
        }
    }
    cout<<"Automata says that the above system call can be from following addresses (virtual):"<<endl;
    for(auto addr: addresses){
        printf("0x%x\t",addr);
    }
    printf("\n");

    set <long int> addr_on_stk;

    std::cout<<"Trace back of the addresses on stack is as follows: "<<std::endl;
    // printf("0x%lx\t", (long)regs.rip);
    addr_on_stk.insert((long)regs.rip);



    if (regs.rbp < regs.rsp){
        //rbp is destroyed by the current function.
        //traversing a depth of 100 words to do best attempt of finding the addresses.
        
        std::cout<<"Oop! rbp is destroyed by the current function!"<<std::endl;
        std::cout<<"Traversing a depth of 2000 words to make best attempt of finding the addresses..."<<std::endl;
    
    __uint128_t pointer=(long)regs.rsp;//getting the stack pointer.
    //trying to get the list of viable return addresses, for those
    //functions in the stack which do not use frame pointer

    
    for(int i=1;i<=2000;i++){
        long viable_ret_addr=ptrace(PTRACE_PEEKDATA, pid, pointer,0);
        if((0xffffff&viable_ret_addr)==viable_ret_addr){
            // printf("0x%lx\t", viable_ret_addr);
            addr_on_stk.insert(viable_ret_addr);
        }
        //printf("pointer = 0x%lx\n",pointer);
        pointer+=8;
        if(pointer>0xffffffffffffffff)
            break;
        }
        // std::cout<<std::endl;   
    }
    else{
        //rbp not destroyed.
        //but functions might not use frame pointer, so traversing stack from rsp to rbp 
        //to check addresses.
        __uint128_t pointer=(long)regs.rsp;//getting the stack pointer.
        //trying to get the list of viable return addresses, for those
        //functions in the stack which do not use frame pointer

        long diff=((long)regs.rbp-pointer)/8;
        long limit=10000;
        if(diff>limit){
            cout<<"rbp and rsp off by more than "<< 8*limit<<" bytes!!! possible stack smash!"<<endl;
            cout<<"Unrolling just by "<<limit<<" words in the stack!"<<endl;

            long upperLimit=pointer+8*limit;

            while(pointer<limit){
                long viable_ret_addr=ptrace(PTRACE_PEEKDATA, pid, pointer,0);
                if((0xffffff&viable_ret_addr)==viable_ret_addr){
                    //printf("0x%lx\t", viable_ret_addr);
                    addr_on_stk.insert(viable_ret_addr);

                }
                //printf("pointer = 0x%lx\n",pointer);
                pointer+=8;
            }    
        }
        else{
            while(pointer<(long)regs.rbp){
                long viable_ret_addr=ptrace(PTRACE_PEEKDATA, pid, pointer,0);
                if((0xffffff&viable_ret_addr)==viable_ret_addr){
                    // printf("0x%lx\t", viable_ret_addr);
                    addr_on_stk.insert(viable_ret_addr);

                }
                //printf("pointer = 0x%lx\n",pointer);
                pointer+=8;
            }
                
            long return_addr=ptrace(PTRACE_PEEKDATA, pid, regs.rbp+8,0);
            long next_rbp=ptrace(PTRACE_PEEKDATA, pid, regs.rbp,0);
            while(next_rbp>0){
                // printf("rbp  = 0x%lx", next_rbp);
                // getchar();
                // printf("0x%lx\t", return_addr);
                addr_on_stk.insert(return_addr);

                return_addr=ptrace(PTRACE_PEEKDATA, pid, next_rbp+8,0);
                next_rbp=ptrace(PTRACE_PEEKDATA, pid, next_rbp,0);
            }
        }
    }

    displayAddress(addr_on_stk);

   
    for (auto x: addresses){
        for(auto y: addr_on_stk){
            if(x==y){
                printf("Match found! Possible address : 0x%x\n",x);
                return true;
            }
        }
    }
    printf("No Matching address found!!\n");
    return false;
                

}

int
main(int argc, char **argv)
{
    if (argc <= 2)
        FATAL("too few arguments: %d", argc);

    //program takes command lines arguments as follows
    //first element is the name of the dot file
    //following it is the executable info and its args

    string dot_file_name(argv[1]);

    parse(dot_file_name);
    initialize_inst_des();

    pid_t pid = fork();
    switch (pid) {
        case -1: /* error */
            FATAL("%s", strerror(errno));
        case 0:  /* child */
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            /* Because we're now a tracee, execvp will block until the parent
             * attaches and allows us to continue. */
            execvp(argv[2], argv + 2);
            FATAL("%s", strerror(errno));
    }

    /* parent */
    waitpid(pid, 0, 0); // sync with execvp
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);

    int i;
    for (i=1;i<=14;i++) {
        /*The first 14 calls are probably specific to linux 
        and has no connection with our program*/
        
        /* Enter next system call */
        if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));
        if (waitpid(pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));

        /* Gather system call arguments */
        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1)
            FATAL("%s", strerror(errno));
        long syscall = regs.orig_rax;

        /* Print a representation of the system call */
        cout<< "Ignoring attempted system call : "<<sys_call_table[syscall]<<endl;
        
        /* Run system call and stop on exit */
        if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));
        if (waitpid(pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));

        /* Get system call result */
        if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1) {
           //fputs(" = ?\n", stderr);
            if (errno == ESRCH)
                exit(regs.rdi); // system call was _exit(2) or similar
            //FATAL("%s", strerror(errno));
        }

        /* Print system call result */
        //fprintf(stderr, " = %ld\n", (long)regs.rax);
        
    }


    for (;;) {
        /* Enter next system call */
        if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));
        if (waitpid(pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));

        /* Gather system call arguments */
        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1)
            FATAL("%s", strerror(errno));
        long syscall = regs.orig_rax;

        /* Print a representation of the system call */

        cout<<"========================================================================================="<<endl;
        cout<<endl;
        cout<< "Attempted to make system call : "<<sys_call_table[syscall]<<endl;
        cout<<endl;





        
        if(!isMovePossible(sys_call_table[syscall], pid, regs) /*&& sys_call_table[syscall] != "exit_group"*/){
            //if (sys_call_table[syscall]=="exit_group" && isMovePossible("exit")){
              //  cout<<"\"exit_group\" requested, but simulating \"exit\" on the automaton!!"<<endl;
            //}else{
                fprintf(stderr, "Illegal system call!! Killing process!\n");
                kill(pid,9);
                return 0;
            //}
        }else
            makeMove(sys_call_table[syscall]);
        
        cout<<"========================================================================================="<<endl;
        cout<<endl;
       
        /* Run system call and stop on exit */
        if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));
        if (waitpid(pid, 0, 0) == -1)
            FATAL("%s", strerror(errno));

        /* Get system call result */
        if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1) {
           //fputs(" = ?\n", stderr);
            if (errno == ESRCH)
                exit(regs.rdi); // system call was _exit(2) or similar
            //FATAL("%s", strerror(errno));
        }

        /* Print system call result */
        //fprintf(stderr, " = %ld\n", (long)regs.rax);
        
    }
}
