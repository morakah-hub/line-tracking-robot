void StartPlan() {
  MotorStop();
  OK(); delay(200);
  //Beep(250);
  Motor(100, 100); delay(150);

  // fline(200,20,100,'l','x');
  // fline(200,20,100,'r','x');
  // Line_Left(250, 'l');
  
  //ine_Left(220, 'l');

 Line_Right(150, 'r');
 Line_Left(150, 'l');
 Line_Timer(150, 120);
Avoid


 /* 
 Line_Left(150, 'l');
 Line_Right(150, 'r');
 Line_Stop(100, 'x');
 Motor(150,150);delay(60);
 TurnR();
Line_Left(150, 'l');
  Line_Right(150, 'r');
 Line_Left(150, 'l');
  Line_Stop(100, 'x');
 Motor(130,130);delay(50);
 TurnL();
 // Line_Left(150, 'l');
  Line_Timer(130, 1600);
  Line_Left(100, 'l');
  Line_Stop(100, 'x');
  Motor(150,150);delay(100);

  /*Line_Left(80, 'l');
   Line_Stop(100, 'x');
  Motor(150,150);delay(100);
 /*Line_Left(150, 'l');
  Line_Right(150, 'r');
  Line_Stop(100, 'x');
   Motor(150,150);delay(60);
   TurnR();
   Line_Left(150, 'l');
  Line_Right(150, 'r');
  Line_Left(150, 'l');
  Line_Left(150, 'l');
  Line_Left(140, 'l');
  Line_Stop(100, 'x');
   Motor(150,150);delay(100);
   //Line_Right(150, 'r');

 /*
 // Line_Right(150, 'r');
 /*
 Line_Stop(100, 'x');
   Motor(150,150);delay(60);
   TurnR();
 
  
  /*
   Line_Left(150, 'l');
 // Line_Left(150, 'l');
  Line_Stop(100, 'x');
   Motor(150,150);delay(60);
   TurnL();
   
  //  Motor(-150,150);delay(130);
  //Line_Left(100, 'l');
  Line_Right(150, 'r');
  Line_Right(150, 'r');
  Line_Right(150, 'r');
  Line_Timer(150, 500);
 Line_Object(120,10,'l');
  Line_Left(150, 'l');
  Line_Right(150, 'r');
  Line_Right(150, 'r');
  Line_Right(150, 'r');
  
  Line_Timer(120, 20);
  Line_Object(100,9,'r');
  Line_Right(150, 'r');
  Line_Left(150, 'l');
Line_Stop(150, 'x');
   Motor(150,150);delay(100);


 
   /*
  Motor(-95,195);delay(250);
  Line_Left(220, 'l');
      Line_Right(250, 'r');
      LineSen_Left(200, 0);
      
       Line_Left(220, 'l');

          Line_Right(250, 'r');
 Line_Stop(250, 'x');
 Motor(195,195);delay(250);
       //LineSen_Left(220, 0);

 // Line_Left(220, 'l');


   /* Line_Left(220, 'l');
Line_Timer(170, 500);
   /*Line_Right(250, 'r');
   
   //Line_Left(220, 'l');
   Line_Right(250, 'r');
      Line_Right(250, 'r');
  Line_Left(220, 'l');
  Line_Left(220, 'l');
      Line_Right(250, 'r');

 /* LineSen_Left(220, 0);
  Line_Left(220, 'l');
  Line_Right(250, 'r');
  Line_Stop(250, 'x');
  //Line_Right(250, 'r');
 // LineSen_Right(
  //Line_Stop(250, 'x');
  /*Line_Left(220, 'l');
  Line_Right(220, 'r');
   Line_Timer(170, 1650);
   Line_Timer(200, 1000);
   Line_Left(220, 'l' );
  Line_Stop(180, 'x');
   Motor(195,-95);delay(250);
     
  Line_Stop(180, 'x');
 /* Line_Object(150,10, 'r');
  
  
 
  Line_Right(220, 'r'); 
  Line_Left(220, 'l');
  Line_Left(220, 'l');
  Line_Timer(150, 1140);
  Line_Object(150,9,'r');
  
  Line_Stop(180, 'x');
  
  Motor(-80,150);delay(250);
  
  Line_Left(250, 'l');
   Line_Right(220, 'r');
    Line_Stop(250, 'x');
  /*
 //Line_TimeLine_Object(150,12, 'r');r(160,2500);
 // Line_Left(250, 'l');
 // Line_Stop(250, 'r');
  
  //Line_Stop(200, 'x');
 // Motor(-80,170);delay(250);
  
 /*
 Line_Object(150,12, 'r');
 Line_Stop(200, 'x');
 Motor(-80,170);delay(250);
    
*/

 
  MotorStop();
  Beep(250);
}


void Sample() {
  MotorStop();
  OK(); delay(500);
  Beep(250);

  Forward(120);
  delay(1000);

  Left(150);
  delay(500);

  Backward(120);
  delay(1000);

  Right(150);
  delay(500);

  MotorStop();
}
