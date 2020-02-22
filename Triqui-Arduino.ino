#include <Elegoo_GFX.h>    // Librería de gráficos propia de la marca de la pantalla TFT.
#include <Elegoo_TFTLCD.h> // Librería de Hardware
#include <TouchScreen.h>   // Librería que permite usar la función Touch de la pantalla.

#define LCD_CS A3  // Pin analógico asignado para selección de chip.
#define LCD_RS A2  // Pin analógico asignado para selección de comandos.
#define LCD_WR A1  // Pin asignado para 'escribir' la señal.
#define LCD_RD A0  // Pin asignado para leer la señal.

#define LCD_RESET A4 // Pin asignado para resetear.

//Códigos definidos para cada color.
#define BLACK   0x0000 
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ILI9341_CYAN        0x07FF      
#define ILI9341_WHITE       0xFFFF 


#define TS_MINX 120
#define TS_MAXX 900
#define TS_MINY 70
#define TS_MAXY 920

#define MINPRESSURE 10 //Mínima presión requerida para que se lea la señal del touch.
#define MAXPRESSURE 1000 //Presión máxima.

//Pines para la función de TouchScreen.
#define YP A3 //Y+
#define XM A2 //X-
#define YM 9  //Y- 
#define XP 8  //X+ 

#define XMARK 1
#define OMARK 0
#define NOMARK -1
#define DRAW -2


//Definición de variables
int moves = 1;
int winner = 0;  
int fbutton_id = 0;
int screen_id = 0;
int clickcounter = 0;
int Board[10]; //Contiene los datos de posición.
int turndicator = OMARK;

Elegoo_TFTLCD tft(LCD_CS, LCD_RS, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


void screen_main()                    //Función para pantalla principal.
{
  screen_id = 0;                      // Se le asigna un ID a cada pantalla. 
  tft.fillScreen(BLACK);              //Pone el fondo de pantalla de el color especificado, en este caso negro.
  tft.drawRect(0,0,240,319,WHITE);    //Dibuja un marco blanco alrededor de la pantalla.
  tft.setCursor(20,110);              //Posición.
  tft.setTextColor(WHITE);            //Color del texto.
  tft.setTextSize(3);                 //Tamaño del texto.
  tft.print("Tic Tac Toe");           //Imprime "Tic Tac Toe" 
  tft.setCursor(75,250);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print("MARIA PAULA REY");
  tft.setCursor(70,265);
  tft.print("ESTEFANIA ZULUAGA");
  tft.fillRect(80, 170, 80, 40, RED);
  tft.setCursor(95, 185);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("PLAY");
    }

void setup(void) {                  //Función para inicializar.
  Serial.begin(9600);
  Serial.print("Starting...");      //Permite identificar en el monitor serie cuando inicia el programa.
  randomSeed(analogRead(0));


  //Permite reconocer la pantalla.
  #ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

   
  uint16_t identifier = tft.readID();
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    identifier=0x9341;}
  tft.reset();
  tft.begin(identifier);
    tft.setRotation(2);
    screen_main();
    }

int CheckWinner() {               //Función que permite identificar un ganador.
  int winner = NOMARK;
  //Compara de un lado a otro si se tiene el mismo simbolo y en caso que si, se asigna el ganador.
  if (Board[1] == Board[2] && Board[1] == Board[3] && winner == NOMARK) {winner=Board[1]; }
  if (Board[4] == Board[5] && Board[4] == Board[6] && winner == NOMARK) {winner=Board[4]; }
  if (Board[7] == Board[8] && Board[7] == Board[9] && winner == NOMARK) {winner=Board[7]; }  

  //Compara hacia abajp si se tiene el mismo simbolo y en caso que si, se asigna el ganador.
  if (Board[1] == Board[4] && Board[1] == Board[7] && winner == NOMARK) {winner=Board[1]; }
  if (Board[2] == Board[5] && Board[2] == Board[8] && winner == NOMARK) {winner=Board[2]; }
  if (Board[3] == Board[6] && Board[3] == Board[9] && winner == NOMARK) {winner=Board[3]; }

  //Compara en diagonal si se tiene el mismo simbolo y en caso que si, se asigna el ganador.
  if (Board[1] == Board[5] && Board[1] == Board[9] && winner == NOMARK) {winner=Board[1]; }
  if (Board[3] == Board[5] && Board[3] == Board[7] && winner == NOMARK) {winner=Board[3]; }
  
  //Finalmente revisa si todos los espacios están ocupados (empate)
  if (winner == NOMARK && Board[1] > -1 && Board[2] > -1 && Board[3] > -1 && Board[4] > -1 && Board[5] > -1 && Board[6] > -1 && Board[7] > -1 && Board[8] > -1 && Board[9] > -1 ) { winner = DRAW; }
  
  return winner;              //Devuelve la variale ganador
}

int GetFingerPos(int x, int y) {   //Obtiene la posición del dedo de modo que el simbolo quede centrado en la casilla.
  int row =0;
  int col =0;
  col = x/80; //Los bloques X son de 80px de ancho.
  row = y/106;
  int pos = row * 3 + col +1 ;
  return pos; 
}

void drawX(int pos) {             //Función para el dibujo de 'X'.
// Draw an X at an arbitrary position on the screen
  tft.drawLine(getCenterX(pos)-30,getCentery(pos)-30, getCenterX(pos)+30, getCentery(pos)+30,WHITE);
  tft.drawLine(getCenterX(pos)+30,getCentery(pos)-30, getCenterX(pos)-30, getCentery(pos)+30,WHITE);
}

void drawO(int pos) {             //Función la el dibujo de 'O'.
  tft.drawCircle(getCenterX(pos),getCentery(pos), 30 ,WHITE);
}

int getCenterX( int pos ) {
if (pos == 1) return 40;
if (pos == 4) return 40;
if (pos == 7) return 40;
if (pos == 2) return 120;
if (pos == 5) return 120;
if (pos == 8) return 120;
if (pos == 3) return 200;
if (pos == 6) return 200;
if (pos == 9) return 200;
}

int getCentery ( int pos ) {
if (pos == 1) return 53;
if (pos == 2) return 53;
if (pos == 3) return 53;
if (pos == 4) return 159;
if (pos == 5) return 159;
if (pos == 6) return 159;
if (pos == 7) return 265;
if (pos == 8) return 265;
if (pos == 9) return 265;
}

void ResetGame() {
  screen_main(); // Entra el menu principal.
  for (int i=0;i<10;i++) { Board[i] = NOMARK; }
}

void playgame() {
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  TSPoint p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
  int pos = GetFingerPos(p.x,p.y);
    if (Board[pos] == -1) {
      if (turndicator == OMARK ) {
        drawO(pos);
        Board[pos] = turndicator;
        turndicator = XMARK;
      } else {
        drawX(pos);
        Board[pos] = turndicator;
        turndicator = OMARK;
      }
      int winner = CheckWinner();
      if (winner != NOMARK) {
        if (winner == XMARK) { 
          tft.setCursor(20,152);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print("X WINS");} 
        if (winner == OMARK) { 
          tft.setCursor(22,152);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print("O WINS"); } 
        if (winner == DRAW) { 
          tft.setCursor(35,152);
          tft.setTextColor(WHITE);
          tft.setTextSize(3);
          tft.print("DRAW");
          } 
        delay(2000);
        ResetGame();
}}}

void screen_board(){
  screen_id = 1;
  tft.fillScreen(BLACK);
  tft.drawLine(0,106,239,106,WHITE);  
  tft.drawLine(0,212,239,212,WHITE);  
  tft.drawLine(80,0,80,319,WHITE);  
  tft.drawLine(160,0,160,319,WHITE); 
  drawX(5);
  delay(500);
  drawO(1);
  delay(500);
  drawX(4);
  delay(500);
  drawO(3);
  delay(500);
  drawX(6);
  playgame();
}

void screen_gameover() {
  screen_id = 2;
  tft.fillScreen(BLACK);
  tft.drawRect(0,0,240,319,WHITE);    
  tft.fillRect(80, 170, 80, 40, RED);
  tft.setCursor(85,175);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("YOU WIN");  
  tft.fillRect(80, 170, 80, 40, RED);
  tft.setCursor(95, 185);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("PLAY AGAIN");
}

void loop(){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  //Raw ADC value
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    if (screen_id == 0) {   //Función touch en la pantalla principal.
      if (p.x > 80 && p.x < 170 && p.y < 220 && p.y > 130){ 
        fbutton_id = 1;
        clickcounter++;
        screen_board();
        } else if (screen_id == 1) { 
        clickcounter++;
       }
      else if (screen_id == 2) { 
      if (p.x > 80 && p.x < 170 && p.y < 220 && p.y > 130){
        fbutton_id = 1;
        clickcounter++;
        screen_main();
        }}
      }}}
}      
      
