/**
 * Spezielle Bytes
 */
#define MESSAGE_START  0x7F
#define MESSAGE_END    0x7E
#define MESSAGE_ESCAPE 0x7D
#define MESSAGE_XOR    0x20
#define MESSAGE_ACK    0xFF



/**
 * Finite-State-Machine Initialisierung
 */
#define STATE_WAIT_START 0  // Warten auf Start-Byte
#define STATE_RECEIVE    1  // Byte empfangen
#define STATE_XOR        2  // Byte nach Escape Byte empfangen
#define EVENT_START      0  // Start  Byte Empfangen
#define EVENT_END        1  // End    Byte Empfangen
#define EVENT_ESCAPE     2  // Escape Byte Empfangen
#define EVENT_OTHER      3  //        Byte Empfangen

byte currentState = STATE_WAIT_START; // Auf Start-Byte warten
char buffer[10];       // Maximale Länge der Nachrichten exkl. Start/End-Bytes
char bufferLength = 0; // Länge der Nachricht

typedef void (*action)(char c);



/**
 * State-Event-Matrix
 */
typedef struct {
  byte nextState;       
  action actionToDo; // function-pointer to the action that shall be released in current state
} stateElement;      // structure for the elements in the state-event matrix

stateElement stateMatrix[3][4] = {
   { {STATE_RECEIVE, taskClear}, {STATE_WAIT_START, taskIdle},  {STATE_WAIT_START, taskIdle}, {STATE_WAIT_START, taskIdle} },
   { {STATE_RECEIVE, taskIdle},    {STATE_WAIT_START, taskReady}, {STATE_XOR, taskIdle},        {STATE_RECEIVE, taskAdd} },
   { {STATE_RECEIVE, taskAddXor},  {STATE_RECEIVE, taskAddXor},   {STATE_RECEIVE, taskAddXor},  {STATE_RECEIVE, taskAddXor} }
};



/**
 * Aktuellen Task ausführen und nächsten Zustand holen
 */
void stateNext(byte e, char c) {
    //determine the State-Matrix-Element in dependany of current state and triggered event
    stateElement stateEvaluation = stateMatrix[currentState][e];
 
    // get next state
    currentState = stateEvaluation.nextState;
    
    // run task
    (*stateEvaluation.actionToDo)(c);
}



/**
 * Serielle Daten empfangen
 */
void receiveSerial() {
  
  char c, e;

  while (Serial.available()) {
    c = Serial.read();
    
    switch (c) {
      case MESSAGE_START:  e = EVENT_START;  break;
      case MESSAGE_END:    e = EVENT_END;    break;
      case MESSAGE_ESCAPE: e = EVENT_ESCAPE; break;
      default:             e = EVENT_OTHER;
    }
    
    stateNext(e, c);
  }
}



/**
 * State-Event-Matrix-Tasks
 */

// Nichts tun
void taskIdle(char c) {}

// Zurücksetzen des Buffers
void taskClear(char c) {
  bufferLength = 0;
}

// Alle Bytes empfangen
void taskReady(char c) {
  evalCommand();
}

// Byte zu Buffer hinzufügen
void taskAdd(char c) {
  buffer[bufferLength++] = c;
}

// Byte mit XOR zu Buffer hinzufügen
void taskAddXor(char c) {
  buffer[bufferLength++] = c ^ MESSAGE_XOR;
}



/**
 * Befehl ausführen
 */
void evalCommand() {
  switch (buffer[0]) {
    case MATRIX_STATE_OFF:
      matrix.state = buffer[0];
      break;

    case MATRIX_STATE_FILL:
      matrix.state = buffer[0];
      matrix.rgb.r = buffer[1];
      matrix.rgb.g = buffer[2];
      matrix.rgb.b = buffer[3];
      matrix.hsv.v = buffer[4];
      matrix.done  = false;
      break;

    case MATRIX_STATE_PLASMA:
      matrix.state = buffer[0];
      matrix.speed = buffer[1];
      break;

    case MATRIX_STATE_FLASH:
      // Alten Zustand retten, solang es nicht auch Flash ist
      if (matrix.state != MATRIX_STATE_FLASH) matrixTemp = matrix;
 
      matrix.state =  buffer[0];
      matrix.rgb.r =  buffer[1];
      matrix.rgb.g =  buffer[2];
      matrix.rgb.b =  buffer[3];
      matrix.number = buffer[4] * 2; // 2 times = fadein+fadeout
      matrix.speed =  buffer[5];
      break;

    case MATRIX_STATE_RAINBOW:
      matrix.state =  buffer[0];
      matrix.hsv.h =  buffer[1];
      matrix.hsv.s =  buffer[2];
      matrix.hsv.v =  buffer[3];
      matrix.effect = buffer[4];
      matrix.speed =  buffer[5];
      break;
  }
}
