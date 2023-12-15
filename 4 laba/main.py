import aiohttp
import asyncio
import os
import tkinter as tk
from tkinter import scrolledtext

class PortScannerGUI:
    def __init__(self, master):
        self.master = master
        master.title("IP Scanner")

        self.text_area = scrolledtext.ScrolledText(master, width=50, height=20)
        self.text_area.pack(padx=10, pady=10)

        self.ip_label = tk.Label(master, text="Enter IP Addresses (comma-separated):")
        self.ip_label.pack(pady=5)
        self.ip_entry = tk.Entry(master)
        self.ip_entry.pack(pady=5)

        self.port_label = tk.Label(master, text="Enter Ports (comma-separated):")
        self.port_label.pack(pady=5)
        self.port_entry = tk.Entry(master)
        self.port_entry.pack(pady=5)
        
        self.scan_button = tk.Button(master, text="Scan", command=self.scan_ports)
        self.scan_button.pack(pady=10)
        
        self.save_button = tk.Button(master, text="Save Results", command=self.save_results)
        self.save_button.pack(pady=10)

    async def scan_port(self, ip_address, port):
        try:
            reader, writer = await asyncio.open_connection(ip_address, port)
            writer.close()
            return port, True
        except (asyncio.TimeoutError, OSError):
            return port, False

    async def scan_ports_async(self, ip_address, ports=None):
        open_ports = []

        if not ports:
            ports = range(1, 2**14)

        tasks = [self.scan_port(ip_address, port) for port in ports]
        results = await asyncio.gather(*tasks)

        for port, is_open in results:
            if is_open:
                open_ports.append(port)

        return open_ports

    async def main_async(self):
        ip_addresses_str = self.ip_entry.get()
        user_specified_ports_str = self.port_entry.get()

        ip_addresses = [ip.strip() for ip in ip_addresses_str.split(',')]

        user_specified_ports = [int(port.strip()) for port in user_specified_ports_str.split(',') if port.strip().isdigit()]
        for ip_address in ip_addresses:
            ip_info = await self.get_ip_info(ip_address)
            if ip_info:
                self.text_area.insert(tk.END, "\nIP Information:\n")
                self.text_area.insert(tk.END, f"IP Address: {ip_info['ip']}\n")
                self.text_area.insert(tk.END, f"Location: {ip_info['city']}, {ip_info['region']}, {ip_info['country']}\n")
                self.text_area.insert(tk.END, f"ISP: {ip_info['org']}\n")
                if 'asn' in ip_info:
                    self.text_area.insert(tk.END, f"AS: {ip_info['asn']}\n")
                else:
                    self.text_area.insert(tk.END, "AS information not available\n")
                if 'hostname' in ip_info:
                    self.text_area.insert(tk.END, f"Hostname: {ip_info['hostname']}\n")
                else:
                    self.text_area.insert(tk.END, "Hostname information not available\n")

            open_ports = await self.scan_ports_async(ip_address, user_specified_ports)
            self.text_area.insert(tk.END, f"Available Ports: {len(open_ports)}\n")
            self.text_area.insert(tk.END, f"Open ports: {f' '.join(map(str, open_ports))}\n")

    def scan_ports(self):
        loop = asyncio.get_event_loop()
        loop.run_until_complete(self.main_async())

    async def get_ip_info(self, ip_address):
        url = f"http://ipinfo.io/{ip_address}/json"
        async with aiohttp.ClientSession() as session:
            async with session.get(url) as response:
                if response.status == 200:
                    ip_info = await response.json()
                    return ip_info
                else:
                    return None
                
    def save_results(self):
        results_text = self.text_area.get("1.0", tk.END)

        script_directory = os.path.dirname(os.path.abspath(__file__))

        file_path = os.path.join(script_directory, "results.txt")
        with open(file_path, "w") as file:
            file.write(results_text)
if __name__ == "__main__":

    root = tk.Tk()
    app = PortScannerGUI(root)
    root.mainloop()
