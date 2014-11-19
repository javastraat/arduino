import processing.serial.*;

class ColorduinoDevice implements Device, StandaloneDevice {

	PApplet app;
        byte curError = 0;

	int baud = 57600;

	Serial port;
	String port_name;

 private byte SYNC_BYTE = (byte)0xaa;
 private byte OPC_PING = (byte)0x01;
 private byte OPC_PLAY_FRAME = (byte)0x02;
 private byte OPC_QUEUE_FRAME = (byte)0x03;
 private byte OPC_SET_MODE = (byte)0x04;
 private byte OPC_FILL = (byte)0x05;
 
  byte  CMODE_PLASMA = 1;
  byte  CMODE_PLAY_FRAME = 2;
  byte  CMODE_MUSIC_SYNC = 3;
  byte  CMODE_FILL = 4;


	// this gamma table is for a  a color resolution of 4096 colors (12bit)
	// taken from RainbowduinoHelper by neophob.com part of neorainbowduino
	private int[] gammaTab = {       
		0,      0,      0,      0,      0,      0,      0,      0,
		0,      0,      0,      0,      0,      0,      0,      0,
		0,      0,      0,      0,      0,      0,      0,      0,
		0,      0,      0,      0,      0,      0,      0,      0,
		0,      0,      0,      0,      0,      0,      0,      0,
		0,      0,      0,      0,      16,     16,     16,     16,
		16,     16,     16,     16,     16,     16,     16,     16, 
		16,     16,     16,     16,     16,     16,     16,     16, 
		16,     16,     16,     16,     16,     16,     16,     16,
		16,     16,     16,     16,     16,     16,     16,     16,
		32,     32,     32,     32,     32,     32,     32,     32, 
		32,     32,     32,     32,     32,     32,     32,     32, 
		32,     32,     32,     32,     32,     32,     32,     32, 
		32,     32,     32,     32,     32,     32,     32,     32, 
		32,     32,     32,     32,     48,     48,     48,     48, 
		48,     48,     48,     48,     48,     48,     48,     48, 
		48,     48,     48,     48,     48,     48,     48,     48, 
		48,     48,     48,     48,     64,     64,     64,     64, 
		64,     64,     64,     64,     64,     64,     64,     64, 
		64,     64,     64,     64,     64,     64,     64,     64, 
		64,     64,     64,     64,     64,     64,     64,     64, 
		80,     80,     80,     80,     80,     80,     80,     80, 
		80,     80,     80,     80,     80,     80,     80,     80, 
		96,     96,     96,     96,     96,     96,     96,     96, 
		96,     96,     96,     96,     96,     96,     96,     96, 
		112,    112,    112,    112,    112,    112,    112,    112, 
		128,    128,    128,    128,    128,    128,    128,    128, 
		144,    144,    144,    144,    144,    144,    144,    144, 
		160,    160,    160,    160,    160,    160,    160,    160, 
		176,    176,    176,    176,    176,    176,    176,    176, 
		192,    192,    192,    192,    192,    192,    192,    192, 
		208,    208,    208,    208,    224,    224,    224,    224, 
		240,    240,    240,    240,    240,    255,    255,    255 
	};


public boolean running;

  int bright = 4;

	public boolean connected() {
  return (port != null);
}

  // return -1 if timed out
  // wait for response code
  public int waitForResponse()
  {
    int cnt = 100; // max wait time in ms
    while (port.available() < 1) {
      delay(1);
      if (cnt-- <= 0) {
        //print("tmo");
        return -1; // timeout
      }
    }
    curError = (byte)port.read();
    return curError; // return error code
  }
  
// ping the colorduino
// return = -1 = no answer
//        else current error code
public int ping() {
    byte pkt[] = new byte[4];
    pkt[0] = SYNC_BYTE;
    pkt[1] = OPC_PING;
    pkt[2] = 0; // data len
    pkt[3] = (byte)(OPC_PING); // chksum
    port.clear();
    port.write(pkt);
    int rc = waitForResponse();
  //  print("pingrc");print(rc);
    return rc;
}
    /**
     * Open serial port with given name. Send ping to check if port is working.
     * If not port is closed and set back to null
     * 
     * @param port_name port to open
     * @param check whether to perform valid checks
     * @return whether port could be opened sucessfully 
     */
    private boolean openPort(String _port_name, boolean check) {
      //      port_name = _port_name;
      //try to find the port
      String[] ports = Serial.list();
      for (int i=0; port==null && i<ports.length; i++) {
	try {
	  port_name = ports[i];
	  port = new Serial(app, port_name, this.baud);

	  port.buffer(1);
	  delay(250); //give it time to initialize		
// n.b. if running w/o bootloader (i.e. using external programmer, you can use short delay below
//	  delay(100); //give it time to initialize		
	  if (ping() == -1) {
	    // no answer
    	    port.stop();
	    port = null;
	  }
	  else {
	    // found one
            print("Found a Colorduino on: ");
            println(port_name);
	    return true;
	  }
	  
	  //catch all, there are multiple exception to catch (NoSerialPortFoundException, PortInUseException...)
	} catch (Exception e) {
	  // search next port...
	}
      }
      
      if (port==null) {
	println("No Colorduino found!");
      }
      return false;
    }


    public void initPort(String _port_name, int _baud, boolean check) {
	if(_baud > 0) this.baud = _baud;
        openPort(_port_name, check);
    }


  ColorduinoDevice(PApplet app) {
    this(app, null, 0);
  }

  ColorduinoDevice(PApplet app, String port_name) {
    this(app, port_name, 0);
  }

  ColorduinoDevice(PApplet app, int baud_rate) {
    this(app, null, baud_rate);
  }

  ColorduinoDevice(PApplet _app, String port_name, int baud_rate) {
  this.app = _app;
    this.initPort(port_name, baud_rate, true);
    //colorduino.reset();
    running = false;
  }

  void setColorScheme() {
 //notyet
/*   PixelColorScheme.R = new int[]{
      0,255        };   
    PixelColorScheme.G = new int[]{
      0,255        };   
    PixelColorScheme.B = new int[]{
      0,255        };   
      */
  }

  boolean draw_as_circle() {
//scl    return true;
	return false;
  }

  boolean enabled() {
    return connected();
  }    

    private void send(int value) {
	if(!connected()) return;
	port.write((byte)value);
    }

  /* +++++++++++++++++++++++++++ */
  public void write_frame(Frame frame) {    
    write_frame(0, frame);
  }

public int setModePlasma() {
  return set_mode(CMODE_PLASMA);
}

public int setModeMusicSync() {
  return set_mode(CMODE_MUSIC_SYNC);
}

public int setModePlayFrame() {
  return set_mode(CMODE_PLAY_FRAME);
}

public int setModeFill() {
  return set_mode(CMODE_FILL);
}
  
  public int set_mode(byte mode) {
    byte pkt[] = new byte[5];
    pkt[0] = SYNC_BYTE;
    pkt[1] = OPC_SET_MODE;
    pkt[2] = 1; // data len
    pkt[3] = mode;
    pkt[4] = (byte)(pkt[1]+pkt[2]+pkt[3]); // chksum
    port.clear();
    port.write(pkt);
    int rc = waitForResponse();
    return rc;
  }

//set rotate to 1 if your matrix is rotated 90deg CW  
int rotate = 1;
  public void write_frame(int num, Frame frame) {  
    if(frame == null || running || !enabled() ) return;
    
    byte pkt[] = new byte[100];
    int idx = 0;
    pkt[idx++] = SYNC_BYTE; // sync
    pkt[idx++] = (current_mode == mode_music) ? OPC_QUEUE_FRAME : OPC_PLAY_FRAME; // frame opcode
    pkt[idx++] = 96; // datalen
    byte chksum = (byte)(pkt[1]+pkt[2]);
    if (rotate == 1) {
//    for (int y=frame.rows-1;y >=0; y--) {
   for (int y=0;y < frame.rows;y++) {
	for (int x=0;x < frame.cols;x++) {
	    PixelColor p = frame.get_pixel(y,x);
	    int r4,g4,b4;
	    int b;

	    // cvt to 4 bit
	 //   r4 = (gammaTab[p.r] >> 4) & 0x0f;
	//    g4 = (gammaTab[p.g] >> 4) & 0x0f;
	 //   b4 = (gammaTab[p.b] >> 4) & 0x0f;
	    r4 = (p.r >> 4) & 0x0f;
	    g4 = (p.g >> 4) & 0x0f;
	    b4 = (p.b >> 4) & 0x0f;

	    b = (r4 << 4) | g4;
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = b4 << 4;

	    p = frame.get_pixel(y,++x);
	    // cvt to 4 bit
	    r4 = (gammaTab[p.r] >> 4) & 0x0f;
	    g4 = (gammaTab[p.g] >> 4) & 0x0f;
	    b4 = (gammaTab[p.b] >> 4) & 0x0f;


	    b |= r4;
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = (g4 << 4) | b4;
	    chksum += b;
	    pkt[idx++] = (byte)b;
}
    }
    }
    else {
    // n.b. colorduino origin is bottom left, our origin is upper left,
    // so loop backwards on the y
    for (int y=frame.rows-1;y >=0; y--) {
	for (int x=0;x < frame.cols;x++) {
	    PixelColor p = frame.get_pixel(x,y);
	    int r4,g4,b4;
	    int b;

	    // cvt to 4 bit
	 //   r4 = (gammaTab[p.r] >> 4) & 0x0f;
	//    g4 = (gammaTab[p.g] >> 4) & 0x0f;
	 //   b4 = (gammaTab[p.b] >> 4) & 0x0f;
	    r4 = (p.r >> 4) & 0x0f;
	    g4 = (p.g >> 4) & 0x0f;
	    b4 = (p.b >> 4) & 0x0f;

	    b = (r4 << 4) | g4;
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = b4 << 4;

	    p = frame.get_pixel(++x,y);
	    // cvt to 4 bit
	    r4 = (gammaTab[p.r] >> 4) & 0x0f;
	    g4 = (gammaTab[p.g] >> 4) & 0x0f;
	    b4 = (gammaTab[p.b] >> 4) & 0x0f;


	    b |= r4;
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = (g4 << 4) | b4;
	    chksum += b;
	    pkt[idx++] = (byte)b;

/*
	    int b;
	    b = (p.r & 0xf0)|((p.g >> 4) & 0x0f);
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = (p.b & 0xf0);
	    p = frame.get_pixel(++x,y);

	    b |= ((p.r >> 4) & 0x0f);
	    chksum += b;
	    pkt[idx++] = (byte)b;
	    b = (p.g & 0xf0)|((p.b >> 4) & 0x0f);
	    chksum += b;
	    pkt[idx++] = (byte)b;
*/
}
	}
    }

    pkt[idx] = chksum;
    port.clear(); // empty the input buffer
    port.write(pkt);
    
    int rc = waitForResponse();
    if ((current_mode == mode_music) && (rc == -1)) {
	// timed out means frame queue is full.
	// tell draw() not to send the next frame until we
	// get a response
	wait_frame_queue_empty = true;
    }
  
   // print("frc");print(rc);
  }

  public void write_matrix(Matrix matrix) {
    print("Start Writing Matrix - ");
    for(int f = 0; f < matrix.num_frames(); f++) {
      write_frame(f, matrix.frame(f));
    }
    println("Done");
  }

  public Matrix read_matrix() {
    Matrix matrix = new Matrix(8,8);
    /*
    print("Start Reading Matrix - ");    
    int frames =  colorduino.bufferLoad(); //return num length
    println( "Frames:" + frames);

    for( int frame_nr = 0; frame_nr < frames; frame_nr++ ) {    
      //println("Frame Nr: " + frame_nr);
      Frame frame = matrix.current_frame();
      int frame_byte[] = colorduino.bufferGetAt(frame_nr);
      for(int y = 0; y < 8; y++ ) {
        for(int x = 0; x < 8; x++ ) {
          frame.set_pixel(x,y, new PixelColor((frame_byte[3*y+0] >> x) & 1, (frame_byte[3*y+1] >> x) & 1, (frame_byte[3*y+2] >> x) & 1 ) );          
        }
      }      
      matrix.add_frame();
    }           
    matrix.delete_frame();
    println("Done");
    */
    return matrix;
  }

  public void toggle() {
    if(running) {
      running = false;
      //      colorduino.reset();
      return;
    } 
    running = true;
    //    colorduino.bufferLoad();
    //    colorduino.start();   
  }

  public void speedUp() {
      //    colorduino.speedUp();
  }

  public void speedDown() {
      //    colorduino.speedDown();
  }

  public void brightnessUp() {
      /*
    this.bright++;
    colorduino.brightnessSet(this.bright);
      */
  }

  public void brightnessDown() {
      /*
    this.bright--;
    if(this.bright < 1) this.bright = 1;
    colorduino.brightnessSet(this.bright);
      */
  }
}









