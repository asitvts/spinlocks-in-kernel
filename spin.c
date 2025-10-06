#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kthread.h>	// for threads
#include <linux/delay.h>	// for sleep


#define NUM_THREADS 10


MODULE_AUTHOR("asit");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("spinlock code");


static int glob=0;
static spinlock_t my_spinlock;

static struct task_struct *threads[NUM_THREADS];	// threads pointer holder


static void critical_section(void){	// critical section function
	
	unsigned long flags;
	
	spin_lock_irqsave(&my_spinlock,flags);
	
	printk(KERN_INFO "critical section incrementing the global variable by thread %s\n", current->comm);
	glob++;
	printk(KERN_INFO "glob: %d\n",glob);
	
	spin_unlock_irqrestore(&my_spinlock,flags);
	
	
	return;
	
}


static int thread_func(void* data){

	int id = *((int*)data);
	kfree(data);
	
	printk(KERN_INFO "thread no %d is entering the critical section\n", id);
	
	while(!kthread_should_stop()){
	
		critical_section();
		
		msleep(1000);
	
	}
	
	
	return 0;

}


static int __init my_module(void){


	spin_lock_init(&my_spinlock);
	
	
	for(int i=0; i<NUM_THREADS; i++){
		
		int* id = kmalloc(1*sizeof(int), GFP_KERNEL);
		if (!id) {
		    printk(KERN_ERR "Failed to allocate memory for thread id %d\n", i);
		    continue;
		}
		*id=i;
		
		threads[i] = kthread_run(thread_func, id, "my_thread_%d", i);
		
		if(IS_ERR(threads[i])){
			printk(KERN_INFO "error making this thread\n");
			threads[i]=NULL;
		}
 	}
	
	
	
	return 0;

}

static void __exit cleanup(void){
	
	printk(KERN_INFO "cleaning up and stopping threads\n");
	
	for(int i=0; i<NUM_THREADS; i++){
		
		if(threads[i]){
			kthread_stop(threads[i]);
			threads[i]=NULL;
		}
		
 	}
 	
	printk(KERN_INFO "clean up done\n");
}


module_init(my_module);
module_exit(cleanup);




