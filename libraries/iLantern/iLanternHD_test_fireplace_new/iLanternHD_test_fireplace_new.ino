void setup() {

  Serial.begin(9600); 
  Serial.print("Setup...");

}


void loop() {

//  Serial.print("Loop...\n");

    int transform_map[25*12]; 
    Serial.print("\nTransformed Map..\n");
    
    init_transform_map(25, 12, transform_map);
    Serial.print("\nInit'ed Transform Map..\n");

    fire(0, 25, 12, transform_map);  
    Serial.print("\nFire..\n");
    

}

void init_colortab(unsigned char colortab[6144])
{
  Serial.print("Colortab..");
        int i, r, g, b;

        for (i=0; i<2048; i++)
        {
                if (i<256)
                {
                        r = i*3;
                        g = (int)(pow((float)i/256., 1.5)*1024.);
                        b = (i*i)/256;
                        if (r > 255) r = 255;
                        if (g > 255) g = 255;
                        if (b > 255) b = 255;
                }
                else
                        r=g=b=0;
                colortab[i*3+1] = r;
                colortab[i*3  ] = g;
                colortab[i*3+2] = b;
        }
}

void apply_gamma(unsigned char colortab[6144], float gamma)
{
  Serial.print("Apply Gamma..");
        int i;
        for (i=0; i<6144; i++)
                colortab[i] = (int)(pow((float)colortab[i]/256., gamma)*256);
}

void init_ca_map(int ca_map[2048])
{
  Serial.print("CA Map..");
        int i, val;

        for(i=0; i<2048; i++)
        {
                val = (int)(pow(i/256.,1.25)/3.7*256.);
                ca_map[i] = val < 2047 ? val : 2047;
        }
}

void fire(int fd, int width, int height, int transform_map[])
{
  Serial.print("Fire");
        char send_buf[height*width*3]; memset(send_buf, 0, sizeof(send_buf));
        int w = width*2, h = height*2; int s=w*(h+1);
        int cells[s]; memset(cells, 0, sizeof(cells));
        int ca_map[2048]; init_ca_map(ca_map);
        unsigned char color_tab[2048*3]; init_colortab(color_tab); apply_gamma(color_tab, 2.2);

        int j, y, i, sum;

        for(;;)
        {
                for (i=w; i<s; i++)
                {
                        sum = cells[i] + cells[i+1];
                        cells[i-w] = ca_map[sum<2047?sum:2047];
                        for (j=w-2, i++; j; j--, i++)
                        {
                                sum = cells[i-1] + cells[i] + cells[i+1];
                                cells[i-w] = ca_map[sum<2047?sum:2047];
                        }
                        sum = cells[i] + cells[i-1];
                        cells[i-1] = ca_map[sum<2047?sum:2047];
                }

                unsigned long noise = rand();
                for (i=s-w; i<s; i++)
                {
                        cells[i] = (noise&1) ? 221 : 0;
                        noise >>= 1;
                }

                for (i=y=0; y<height; y++)
                        for (j=0; j<w; j+=2,i++)
                        {
                                int ix = ( cells[i*4-j] + cells[i*4-j+1] +
                                           cells[i*4+w-j] + cells[i*4+w-j+1] ) >> 2;
                                ix += ix << 1;
                                int map = transform_map[i];
                                map += map << 1;

                                send_buf[map++] = color_tab[ix++];
                                send_buf[map++] = color_tab[ix++];
                                send_buf[map  ] = color_tab[ix  ];
                                
                        }


//                spi_transfer(fd, NULL, send_buf, width*height*3);

//                usec += 20000; usec %= 1000000;
//                gettimeofday(&tv, NULL);
//                wait = 1000000 + usec - tv.tv_usec; wait %= 1000000;
//                if (wait < 20000)
//                        usleep(wait);
        }
          delay(20);
  Serial.print("\nSend Buffer:\n");
  Serial.print(String(send_buf));
}

void init_transform_map(int width, int height, int map[])
{
  Serial.print("Transform Map");
        int x, y;
        for (y=0; y<height; y++)
                for (x=0; x<width; x++)
                        map[y*width+x] = (x&1) ? height*x + y : height*(x+1)-1 - y;
}


