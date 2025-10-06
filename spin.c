#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_AUTHOR("asit");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("spinlock code");


static int glob=0;
static spinlock_t my_spinlock;

static void critical_section(void){

	
	unsigned long flags;
	
	
	spin_lock_irqsave(&my_spinlock,flags);
	
	printk(KERN_INFO "inside critical section incrementing the global variable glob\n");
	glob++;
	printk(KERN_INFO "glob: %d\n",glob);
	
	spin_unlock_irqrestore(&my_spinlock,flags);
	
	return;
	
}


static int __init my_module(void){


	spin_lock_init(&my_spinlock);
	
	critical_section();
	critical_section();
	critical_section();
	
	
	
	return 0;

}

static void __exit cleanup(void){
	
	printk(KERN_INFO "clean up done");
}


module_init(my_module);
module_exit(cleanup);




