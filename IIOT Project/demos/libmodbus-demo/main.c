#include <stdio.h>
#include <errno.h>
#include <modbus.h>

int main() {
    // Configuration parameters
    const char *device = "/dev/ttyUSB2";
    const int baudrate = 9600;
    const char parity = 'N';
    const int bytesize = 8;
    const int stopbits = 1;

    const int slave_num = 0x05;

    // Modbus 'object'
    modbus_t *mb;

    mb = modbus_new_rtu(device, baudrate, parity, bytesize, stopbits);

    if (mb == NULL) {
        fprintf(stderr, "Could not create libmodbus context: %s", modbus_strerror(errno));
        return -1;
    }

    if (modbus_connect(mb) == -1) {
        fprintf(stderr, "Modbus connection failed: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }

    if (modbus_set_slave(mb, slave_num) == -1) {
        fprintf(stderr, "Failed to set modbus slave number: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }


    // ------------------------------------------------
    // Read Registers

    const int num_regs = 4;
    const int start_from = 0x0000;

    uint16_t registers[num_regs * sizeof(uint16_t)];
    if (modbus_read_registers(mb, start_from, num_regs, registers) == -1) {
        fprintf(stderr, "Failed to read holding registers: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }


    struct output {
        uint16_t temp;
        uint16_t hum;
        uint16_t rs485address;
        uint16_t baudrate;
    };
    struct output *o = registers;


    printf("[RAW] 0x0000 Temperature: %d\n", o->temp);
    printf("[RAW] 0x0001 Humidity   : %d\n", o->hum);
    printf("[RAW] 0x0002 Address    : %d\n", o->rs485address);
    printf("[RAW] 0x0003 Baudrate   : %d\n", o->baudrate);

    float temperature, humidity;
    temperature = o->temp * 0.1;
    humidity = o->hum * 0.1;

    printf("[PARSED] Temperature %4.2f °C\n", temperature);
    printf("[PARSED] Humidity    %4.2f %%RH\n", humidity);

    // ------------------------------------------------
    // Write Registers

    // Set baudrate to 19200
    if (modbus_write_register(mb, 0x0003, 4) == -1) {
        fprintf(stderr, "Failed to write baudrate register: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }

    if (modbus_read_registers(mb, start_from, num_regs, registers) == -1) {
        fprintf(stderr, "Failed to read holding registers: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }
    printf("[CHANGE] After changing Baudrate to 19200: \n");
    printf("[RAW] 0x0003 Baudrate   : %d\n", o->baudrate);


    // Set baudrate to 9600
    if (modbus_write_register(mb, 0x0003, 3) == -1) {
        fprintf(stderr, "Failed to write baudrate register: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }

    if (modbus_read_registers(mb, start_from, num_regs, registers) == -1) {
        fprintf(stderr, "Failed to read holding registers: %s", modbus_strerror(errno));
        modbus_free(mb);
        return -1;
    }
    printf("[CHANGE] After changing Baudrate back to 9600: \n");
    printf("[RAW] 0x0003 Baudrate   : %d\n", o->baudrate);

    // Close modbus
    modbus_close(mb);
    modbus_free(mb);
    return 0;
}
