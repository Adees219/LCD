from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QLineEdit, QPushButton, QVBoxLayout, QWidget
from PyQt5.QtCore import QTimer
import serial
serial_port = None
#Una clase que toma las propiedades QMainWindow
class MainWindow(QMainWindow):
    def __init__(self):         #Método de inicalización, con atributo self
        super().__init__()      
        self.setWindowTitle("Serial Port Test")
        self.setGeometry(100, 100, 300, 200)

        # Create the GUI elements
        self.send_button1 = QPushButton("INC")
        self.send_button2 = QPushButton("DEC")
        self.send_button3 = QPushButton("menu")

        self.sent_label = QLabel("")
        self.output_label = QLabel("")


#------------------------------------------------------------
        # crea la ventana y agregamos los elementos
        layout = QVBoxLayout()
        #layout.addWidget(QLabel("Enter text to send:"))
        layout.addWidget(self.send_button1)
        layout.addWidget(self.send_button2)
        layout.addWidget(self.send_button3)
        #layout.addWidget(QLabel("Sent data:"))
        #layout.addWidget(self.sent_label)
        layout.addWidget(QLabel("Valor puerto:"))
        layout.addWidget(self.output_label)

        # Create a widget and set the layout on it
        widget = QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)

        # Coneccion de ambos botones con su funcion de presionado
        self.send_button1.clicked.connect(self.on_send_button_clicked1)
        self.send_button2.clicked.connect(self.on_send_button_clicked2)
        self.send_button3.clicked.connect(self.on_send_button_clicked3)

        # Initialize the serial port to None
        self.serial_port = None

    def on_send_button_clicked1(self): #Función para el botón 1
        if self.serial_port: #Revisamos si hay conexión serial
            message = b"+" #Codificamos en bytes el dato que mandaremos al pic
            self.serial_port.write(message) #Le escribimos el dato el puerto serial
            self.sent_label.setText(message.decode()) #Decodificamos el dato y se lo ingresamos a la etiqueta de sent_data

    def on_send_button_clicked2(self): #Función para el botón 2
        if self.serial_port: #Revisamos si hay conexión serial
            message = b"-" #Codificamos en bytes el dato que mandaremos al pic
            self.serial_port.write(message)  #Le escribimos el dato el puerto serial
            self.sent_label.setText(message.decode()) #Decodificamos el dato y se lo ingresamos a la etiqueta de sent_data
                
    def on_send_button_clicked3(self): #Función para el botón 2
        if self.serial_port: #Revisamos si hay conexión serial
            message = b"m" #Codificamos en bytes el dato que mandaremos al pic
            self.serial_port.write(message)  #Le escribimos el dato el puerto serial
            self.sent_label.setText(message.decode()) #Decodificamos el dato y se lo ingresamos a la etiqueta de sent_data

    def update_data(self):
        self.data = str
        if self.serial_port and self.serial_port.in_waiting>0: #Revisamos si hay conexión serial y si .in_waiting es mayor que cero, significa que hay datos disponibles
            #while( self.serial_port.read().decode().strip() != '\0'):
            
            data = self.serial_port.read().decode().strip() #Leemos los datos y los decodificamos para ingresarlos en la variable data

            #if data.isdigit(): # Verificar si la línea de datos contiene un número
            self.output_label.setText(f"{data}") # Actualizar la etiqueta del contador con el nuevo valor

    def connect_serial_port(self, port, baudrate):
        self.serial_port = serial.Serial(port, baudrate)
        self.statusBar().showMessage(f"Connected to {port} at {baudrate} baud")
        self.serial_port.flushInput()
        self.serial_port.flushOutput()

        
if __name__ == '__main__':
    app = QApplication([])
    window = MainWindow()
    window.show()
    timer = QTimer()
    timer.timeout.connect(window.update_data)
    timer.start(50)
    window.connect_serial_port('COM3', 9600) 
    app.exec_()