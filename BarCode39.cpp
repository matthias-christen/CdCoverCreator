
/////////////////////////////////////////////////////
//  Copyright 2003 by David Hillard                //
//  This information must remain in file           //
//                                                 //
//  Please provide credit for this code in your    //
//  finished application in the Help/About box     //
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarCode39.h"

int BC39_Decode(char letter)
    {
     int code;
     switch(letter)
         {
         case '*':code = 0x0094;break;
         case '1':code = 0x0121;break;
         case '2':code = 0x0061;break;
         case '3':code = 0x0160;break;
         case '4':code = 0x0031;break;
         case '5':code = 0x0130;break;
         case '6':code = 0x0070;break;
         case '7':code = 0x0025;break;
         case '8':code = 0x0124;break;
         case '9':code = 0x0064;break;
         case '0':code = 0x0034;break;
         case 'A':code = 0x0109;break;
         case 'B':code = 0x0049;break;
         case 'C':code = 0x0148;break;
         case 'D':code = 0x0019;break;
         case 'E':code = 0x0118;break;
         case 'F':code = 0x0058;break;
         case 'G':code = 0x000D;break;
         case 'H':code = 0x010C;break;
         case 'I':code = 0x004C;break;
         case 'J':code = 0x001C;break;
         case 'K':code = 0x0103;break;
         case 'L':code = 0x0043;break;
         case 'M':code = 0x0142;break;
         case 'N':code = 0x0013;break;
         case 'O':code = 0x0112;break;
         case 'P':code = 0x0052;break;
         case 'Q':code = 0x0007;break;
         case 'R':code = 0x0106;break;
         case 'S':code = 0x0046;break;
         case 'T':code = 0x0016;break;
         case 'U':code = 0x0181;break;
         case 'V':code = 0x00C1;break;
         case 'W':code = 0x01C0;break;
         case 'X':code = 0x0091;break;
         case 'Y':code = 0x0190;break;
         case 'Z':code = 0x00D0;break;
         case '-':code = 0x0085;break;
         case '.':code = 0x0184;break;
         case ' ':code = 0x00C4;break;
         case '$':code = 0x00A8;break;
         case '/':code = 0x00A2;break;
         case '+':code = 0x008A;break;
         case '%':code = 0x002A;break;



         default: code = 0x0000;//don't barcode invalid characters
         }//end switch
     return code;
    }

void BC39_Expand(int code,char* wn)
    {
         {
         if(code & 0x0100){wn[0]='W';}else{wn[0]='N';}
         if(code & 0x0080){wn[1]='W';}else{wn[1]='N';}
         if(code & 0x0040){wn[2]='W';}else{wn[2]='N';}
         if(code & 0x0020){wn[3]='W';}else{wn[3]='N';}
         if(code & 0x0010){wn[4]='W';}else{wn[4]='N';}
         if(code & 0x0008){wn[5]='W';}else{wn[5]='N';}
         if(code & 0x0004){wn[6]='W';}else{wn[6]='N';}
         if(code & 0x0002){wn[7]='W';}else{wn[7]='N';}
         if(code & 0x0001){wn[8]='W';}else{wn[8]='N';}
         wn[9]='N';
         wn[10]=0x00;

         }

    }


int BC39_Draw(HDC hdc,RECT* rect,char* text,double cpi,BOOL HORIZ)
    {
     char bcarray[400];
     char wn[11];
     char tbuffer[100];
     char textcopy[100];
     int j,code,sl,barwidth,barcodelength,rv;
     int ppi,numchars,lengthinxwidths;
     double xdim,offset;
     HPEN holdpen;
     HBRUSH holdbrush;

     //copy text to textcopy for modification to uppercase
     strcpy(textcopy,text);
     if(strlen(textcopy)>30)
         {
          return 1;
         }
     strcpy(bcarray,"");
     strupr(textcopy);
     code=BC39_Decode('*');
     BC39_Expand(code,wn);
     strcpy(bcarray,wn);
     for(j=0;j<(int)(strlen(textcopy));j++)
         {
          code=BC39_Decode(textcopy[j]);
          if(code)
              {
               BC39_Expand(code,wn);
               strcat(bcarray,wn);
              }
         }
     code=BC39_Decode('*');
     BC39_Expand(code,wn);
     strcat(bcarray,wn);
     //now bcarray holds bar width string; 10 bytes per character
     numchars=(int)(strlen(bcarray)/10);
     lengthinxwidths=numchars*16;




     //calculate the x dimension
     if(HORIZ)
         {
          ppi=GetDeviceCaps(hdc,LOGPIXELSX);
         }
     else
         {
          ppi=GetDeviceCaps(hdc,LOGPIXELSY);
         }
     wsprintf(tbuffer,"ppi = %d",ppi);
     if(cpi<1){cpi=1;}
     xdim=(ppi/(16*cpi)); 
     if(xdim<1){xdim=1;}

     barcodelength=(int)(xdim*lengthinxwidths + xdim*20);
     //adjust rect width if necessary
     rv=0;
     if(HORIZ)
         {
          int diff;
          diff=barcodelength-(rect->right - rect->left);
          if(diff > 0)
              {
               rect->right += diff;
               rv=1;
              }
         }
     else
         {
          int diff;
          diff=barcodelength-(rect->bottom - rect->top);
          if(diff > 0)
              {
               rect->bottom += diff;
               rv=1;
              }
         }


     holdbrush=(HBRUSH)SelectObject(hdc,GetStockObject(BLACK_BRUSH));
     holdpen=(HPEN)SelectObject(hdc,GetStockObject(BLACK_PEN));

     sl=strlen(bcarray);
     if(HORIZ)
         {
          offset=((rect->right - rect->left)/2) - (barcodelength/2);
         }
     else
         {
          offset=((rect->bottom - rect->top)/2) - (barcodelength/2);
         }
     offset+=xdim*10;
     if(HORIZ)
         {
             for(j=0;j<sl;j++)
                 {
                  if(((j/2)*2)==j)
                      {
                       //even is bar
                       if(bcarray[j]=='W')
                           {
                            //draw wide bar
                            barwidth=(int)(xdim*3);
                            Rectangle(hdc,rect->left+(int)offset,rect->top,rect->left+(int)offset+barwidth,rect->bottom);
                            offset+=barwidth;
                           }
                       else
                           {
                            //draw narrow bar
                            barwidth=(int)xdim;
                            Rectangle(hdc,rect->left+(int)offset,rect->top,rect->left+(int)offset+barwidth,rect->bottom);
                            offset+=barwidth;
                           }
               
                      }
                  else
                      {
                       //odd is space
                       if(bcarray[j]=='W')
                           {
                            offset+=(xdim*3);
                           }
                       else
                           {
                            offset+=(xdim);
                           }
                      }

                 }
         }
     else  //not HORIZ
         {
             for(j=0;j<sl;j++)
                 {
                  if(((j/2)*2)==j)
                      {
                       //even is bar
                       if(bcarray[j]=='W')
                           {
                            //draw wide bar
                            barwidth=(int)(xdim*3);
                            Rectangle(hdc,rect->left,rect->top+(int)offset,rect->right,rect->top+(int)offset+barwidth);
                            offset+=barwidth;
                           }
                       else
                           {
                            //draw narrow bar
                            barwidth=(int)xdim;
                            Rectangle(hdc,rect->left,rect->top+(int)offset,rect->right,rect->top+(int)offset+barwidth);
                            offset+=barwidth;
                           }
               
                      }
                  else
                      {
                       //odd is space
                       if(bcarray[j]=='W')
                           {
                            offset+=(xdim*3);
                           }
                       else
                           {
                            offset+=(xdim);
                           }
                      }

                 }

         }
     SelectObject(hdc,holdpen);
     SelectObject(hdc,holdbrush);

     return rv;
    }