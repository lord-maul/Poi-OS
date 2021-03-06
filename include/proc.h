
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


struct stackframe {	/* proc_ptr points here				pr↑ Low			*/
	u32	gs;		/* ┓						│			*/
	u32	fs;		/* ┃						│			*/
	u32	es;		/* ┃						│			*/
	u32	ds;		/* ┃						│			*/
	u32	edi;		/* ┃						│			*/
	u32	esi;		/* ┣ pushed by save()				│			*/
	u32	ebp;		/* ┃						│			*/
	u32	kernel_esp;	/* <- 'popad' will ignore it			│			*/
	u32	ebx;		/* ┃						↑栈从高地址往低地址增长*/		
	u32	edx;		/* ┃						│			*/
	u32	ecx;		/* ┃						│			*/
	u32	eax;		/* ┛						│			*/
	u32	retaddr;	/* return address for assembly code save()	│			*/
	u32	eip;		/*  ┓						│			*/
	u32	cs;		/*  ┃						│			*/
	u32	eflags;		/*  ┣ these are pushed by CPU during interrupt	│			*/
	u32	esp;		/*  ┃						│			*/
	u32	ss;		/*  ┛						┷High			*/
};


struct proc {
	struct stackframe regs;    /* process registers saved in stack frame */

	u16 ldt_sel;               /* gdt selector giving ldt base and limit */
	struct descriptor ldts[LDT_SIZE]; /* local descs for code and data */

    int ticks;                 /* remained ticks */
    int priority;

	u32 pid;                   /* process id passed in from MM */
	char name[16];		   /* name of the process */

	int  p_flags;              /**
				    * process flags.
				    * A proc is runnable iff p_flags==0
				    */

	MESSAGE * p_msg;
	int p_recvfrom;
	int p_sendto;

	int has_int_msg;           /**
				    * nonzero if an INTERRUPT occurred when
				    * the task is not ready to deal with it.
				    */

	struct proc * q_sending;   /**
				    * queue of procs sending messages to
				    * this proc
				    */
	struct proc * next_sending;/**
				    * next proc in the sending
				    * queue (q_sending)
				    */

	struct proc * next_proc;	//就绪队列中的下一个进程
	int already_run_for;		//该进程已经消耗的时间片

	int nr_tty;
};

struct task {
	task_f	initial_eip;
	int	stacksize;
	char	name[32];
};

#define proc2pid(x) (x - proc_table)

/* Number of tasks & procs */
#define NR_TASKS	6
#define NR_PROCS	3
#define NR_APROCQUEUE 1 //第一级用户进程队列中进程数量

#define FIRST_PROC	proc_table[0]
#define LAST_PROC	proc_table[NR_TASKS + NR_PROCS - 1]

//CMOS RTC time
#define CMOS_READ(addr) ({\
	out_byte(0x70,0x80|addr);\
	in_byte(0x71);\
	})


/* stacks of tasks */
#define STACK_SIZE_TTY		0x8000
#define STACK_SIZE_SYS		0x8000
#define STACK_SIZE_SHELL	0x8000
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TESTB	0x8000
#define STACK_SIZE_TESTC	0x8000
#define STACK_SIZE_GPOW		0x8000
#define STACK_SIZE_GTICKS   0x8000
#define STACK_SIZE_TICK 0x8000

#define STACK_SIZE_TOTAL	(STACK_SIZE_TTY + \
				STACK_SIZE_SYS + \
				STACK_SIZE_SHELL + \
				STACK_SIZE_TESTA + \
				STACK_SIZE_TESTB + \
				STACK_SIZE_TESTC + \
				STACK_SIZE_GPOW + \
				STACK_SIZE_GTICKS +\
				STACK_SIZE_TICK)



