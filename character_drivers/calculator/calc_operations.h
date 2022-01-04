#define NO_OF_DEVICES 4
#define data_size 4 //sizeof(int)


int device1_operands[2]={0};
int device2_operands[2]={0};
int device3_operands[2]={0};
int device4_operands[2]={0};


struct dev_private_data {
    const char* name;
    int* buffer;
    unsigned size;
    //uint8_t perm;
    struct cdev kcal_dev;
};

struct driver_private_data{
    int total_devices;
    dev_t device_number;
    struct dev_private_data dev_data[NO_OF_DEVICES];
};


struct driver_private_data drv_prvdata={
    .total_devices=NO_OF_DEVICES,
    .dev_data={

        [0] = {
            .buffer=device1_operands,
            .size=2,
            .name="ADD"
        },

        [1] = {
            .buffer=device2_operands,
            .size=2,
            .name="SUB"
        },

        [2] = {
            .buffer=device3_operands,
            .size=2,
            .name="MUL"
        },

        [3] = {
            .buffer=device4_operands,
            .size=2,
            .name="DIV"
        }

    }
};



static int calc_open(struct inode *inode, struct file *file);

static int  calc_release(struct inode *inode, struct file *file);

static ssize_t Add_Write(struct file *filp, const char *buf, size_t len, loff_t * off);
static ssize_t Add_Read(struct file *filp, char* buf, size_t len, loff_t * off);
static ssize_t Sub_Write(struct file *filp, const char *buf, size_t len, loff_t * off);
static ssize_t Sub_Read(struct file *filp, char* buf, size_t len, loff_t * off);

static ssize_t Mul_Write(struct file *filp, const char *buf, size_t len, loff_t * off);

static ssize_t Div_Write(struct file *filp, const char *buf, size_t len, loff_t * off);


static ssize_t Mul_Read(struct file *filp, char* buf, size_t len, loff_t * off);
static ssize_t Div_Read(struct file *filp, char* buf, size_t len, loff_t * off);


/*
static ssize_t (*write_func_pointers[4]) (struct file *filp, const char *buf, size_t len, loff_t * off)={Add_Write,Sub_Write,Mul_Write,Div_Write};

static ssize_t (*read_func_pointers[4]) (struct file *filp,  char *buf, size_t len, loff_t * off)={Add_Read,Sub_Read,Mul_Read,Div_Read};
*/
