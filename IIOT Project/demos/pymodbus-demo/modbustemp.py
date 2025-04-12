from pymodbus.client import ModbusSerialClient
from pymodbus.framer import ModbusRtuFramer
from pymodbus.exceptions import ModbusException
from serial import SerialException

from rich import print
class ModbusTempSensor():
    BAUDRATE = 9600
    BYTESIZE = 8
    PARITY = 'N'
    STOPBITS = 1
    DEV_ADDR = 0x05
    TEMP_AT = 0x0
    HUM_AT = 0x1

    class NotConnectedError:
        pass

    def __init__(self, port : str):
        try:
            self.cli = ModbusSerialClient(port, framer=ModbusRtuFramer,
                                      baudrate=self.BAUDRATE,
                                      bytesize=self.BYTESIZE,
                                      parity=self.PARITY,
                                      stopbits=self.STOPBITS)
            print("[blue]Connecting with modbus RTU")
            self.cli.connect()
        except ModbusException as exc:
            print("[bold magenta]Modbus exception: {}".format(exc.string))
        except SerialException as exc:
            print("[bold magenta]Serial exception: {}".format(exc.strerror))
        except:
            print("[bold magenta]Unknown error")

    def get_temp(self):
        if self.cli.connected:
            resp = self.cli.read_holding_registers(self.TEMP_AT, 1, self.DEV_ADDR)
            temp = resp.registers[0] * 0.1
            return temp
        else:
            raise self.NotConnectedError

    def get_hum(self):
        if self.cli.connected:
            resp = self.cli.read_holding_registers(self.HUM_AT, 1, self.DEV_ADDR)
            hum = resp.registers[0] * 0.1
            return hum
        else:
            raise self.NotConnectedError

    def get_values(self):
        if self.cli.connected:
            resp = self.cli.read_holding_registers(self.TEMP_AT, 2, self.DEV_ADDR)
            hum = resp.registers[0] * 0.1
            temp = resp.registers[1] * 0.1
            return [hum, temp]
        else:
            raise self.NotConnectedError

    def disconnect(self):
        self.cli.close()