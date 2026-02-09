#include"Lcd.h"


// Función para inicializar el LCD en modo de 4 bits
void lcd_init() {
  // Configuramos los pines como salida para el LCD
  setDDR_RS(1);
  setDDR_RW(1);
  setDDR_E(1);
  setDDR_D4(1);
  setDDR_D5(1);
  setDDR_D6(1);
  setDDR_D7(1);
  setPORT_RW(0); // RW en 0 para escritura
  delay(50);  // Espera a que el LCD se encienda

  // Secuencia de inicialización para modo de 4 bits (según especificación del HD44780)
  lcd_write4bits(0x03);  // Enviar 0x03 tres veces para inicializar
  delay(5);
  lcd_write4bits(0x03);  // Repetir el comando de inicialización
  delay(5);
  lcd_write4bits(0x03);
  delay(5);
  lcd_write4bits(0x02);  // Modo de 4 bits
  delay(5);
  // Configuración del LCD
  lcd_command(0x28);  // Modo de 4 bits, 2 líneas, matriz 5x8
  lcd_command(0x0C);  // Display encendido, sin cursor
  lcd_command(0x06);  // Modo de incremento, sin desplazamiento
  lcd_command(0x01);  // Limpiar la pantalla
  delay(5);           // Espera después de limpiar
}

// Función para enviar comandos al LCD
void lcd_command(uint8_t cmd) {
  setPORT_RS(0);  // RS en LOW para comandos
  lcd_write4bits(cmd >> 4);  // Enviar los 4 bits más significativos
  lcd_write4bits(cmd & 0x0F);  // Enviar los 4 bits menos significativos
  delayMicroseconds(100);
}

// Función para escribir datos (caracteres) en el LCD
void lcd_write(uint8_t data) {
  setPORT_RS(1);  // RS en HIGH para datos
  lcd_write4bits(data >> 4);  // Enviar los 4 bits más significativos
  lcd_write4bits(data & 0x0F);  // Enviar los 4 bits menos significativos
  delayMicroseconds(100);
}

// Función para mover el cursor a una posición específica (columna, fila)
void lcd_setCursor(uint8_t col, uint8_t row) {
  const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  lcd_command(0x80 | (col + row_offsets[row]));  // Mover el cursor
}

// Función para imprimir una cadena de caracteres en el LCD
void lcd_print(const char *str) {
  while(lcd_checkIfBusy());
  while (*str) {
    lcd_write(*str++);
  }
}

// Función para enviar datos de 4 bits al LCD
void lcd_write4bits(uint8_t nibble) {

  setPORT_D4((nibble >> 0) & 0x01);
  setPORT_D5((nibble >> 1) & 0x01);
  setPORT_D6((nibble >> 2) & 0x01);
  setPORT_D7((nibble >> 3) & 0x01);
  
  pulseEnable();
}

// Generar el pulso de habilitación
void pulseEnable() {
  setPORT_E(0);
  delayMicroseconds(1);
  setPORT_E(1);
  delayMicroseconds(1);
  setPORT_E(0);
  delayMicroseconds(100);
}

// Función para limpiar la pantalla
void lcd_clear() {
  lcd_command(0x01);
  delay(2);
}

// Función para mover el cursor a la posición inicial
void lcd_home() {
  lcd_command(0x02);
  delay(2);
}

// Función para encender el display
void lcd_displayOn() {
  lcd_command(0x0C);
}

// Función para apagar el display
void lcd_displayOff() {
  lcd_command(0x08);
}

// Función para activar el cursor
void lcd_cursorOn() {
  lcd_command(0x0E);
}

// Función para desactivar el cursor
void lcd_cursorOff() {
  lcd_command(0x0C);
}

// Función para activar el parpadeo del cursor
void lcd_blinkOn() {
  lcd_command(0x0D);
}

// Función para desactivar el parpadeo del cursor
void lcd_blinkOff() {
  lcd_command(0x0C);
}

// Función para desplazar el display a la izquierda
void lcd_scrollDisplayLeft() {
  lcd_command(0x18);
}

// Función para desplazar el display a la derecha
void lcd_scrollDisplayRight() {
  lcd_command(0x1C);
}

// Función para crear un carácter personalizado
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;  // Limitar a las posiciones de memoria disponibles
  lcd_command(0x40 | (location << 3));
  for (int i = 0; i < 8; i++) {
    lcd_write(charmap[i]);
  }
}

//comprueba si el flag busy esta activo
bool lcd_checkIfBusy(){
  setPORT_RS(0); //poner en modo lectura
  setPORT_RW(1);
  setDDR_D7(0);
  bool result = registerD7 & (1<<D7); //leer el flag busy
  setPORT_RS(1); //poner en modo escritura
  setPORT_RW(0);
  setDDR_D7(1);
  return result;
}
