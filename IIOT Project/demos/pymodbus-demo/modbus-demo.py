import time

from modbustemp import ModbusTempSensor
from rich.panel import Panel
from rich.live import Live
from rich.table import Table


def generate_temp_view(temp, hum) -> Panel:
    table = Table()
    table.add_column("Measurement")
    table.add_column("Value")
    table.add_column("Unit")

    table.add_row("Temperature", f"{temp:4.2f}", "[cyan]°C")
    table.add_row("Humidty", f"{hum:4.2f}", "[cyan]%RH")

    panel = Panel(table,
                  title="Showing live values from modbus holding registers")
    return panel

def main():
    sensor = ModbusTempSensor("/dev/ttyUSB0")

    with Live(generate_temp_view(0, 0), refresh_per_second=4) as live:
        while True:
            time.sleep(2)
            t, h = sensor.get_values()
            live.update(generate_temp_view(t, h))


if __name__ == '__main__':
    main()