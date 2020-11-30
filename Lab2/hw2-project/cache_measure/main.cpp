#include <linux/init.h>
#include <linux/module.h>

static int disableCache_init(void)
{
        printk(KERN_ALERT "Disabling L1 and L2 caches.\n");
        __asm__(".intel_syntax noprefix\n\t"
                "mov    eax,cr0\n\t"
                "or     eax,(1 << 30)\n\t"
                "mov    cr0,eax\n\t"
                "wbinvd\n\t"
                ".att_syntax noprefix\n\t"
        : : : "eax" );
        return 0;
}
static void disableCache_exit(void)
{
        printk(KERN_ALERT "Enabling L1 and L2 caches.\n");
        __asm__(".intel_syntax noprefix\n\t"
                "mov    eax,cr0\n\t"
                "and     eax,~(1 << 30)\n\t"
                "mov    cr0,eax\n\t"
                "wbinvd\n\t"
                ".att_syntax noprefix\n\t"
        : : : "eax" );
}

const int N = 100000;
int a[N];

int main(){
	disableCache_init();
	int val = 0;
	for(int i = 0; i < N; i++)
		val += a[i] + 1;
	disableCache_exit();
	return 0;
}