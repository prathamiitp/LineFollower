/* using 6IR sensors and this code, we can run at fast speed in st lines, and 
at low speed at junctions or turns. also it can cross breaking lines at low speed.
It also can adjust to change in contrast of line and track in both continious line and breaking lines.
This code takes appropriet hard turns at L-junctions, appropriet soft turns at obtuse junctions,
left turn at + junction, U turn at dead ends, and can stop at T-junction. */



// saving some time
#define dr digitalRead

/* defining the motors as mot- motor, 
R- right, L- left, A- anticlockwise, C- clockwise*/
#define motRC  8
#define motRA  9
#define motLC  10
#define motLA  11

/* defining the sensors (L2 and L1 on left)
(M1 in middle, M2 in middle front)(R1 and R2 on right)*/
#define M2  A0
#define L2  A1
#define L1  A2 
#define M1  A3
#define R1  A4
#define R2  A5

int line; //to dynamically define what color is line and what colour is background.

void setup() {

  	Serial.begin(9600);

  	// defining outputs
  	pinMode(motRC, OUTPUT);
  	pinMode(motRA, OUTPUT);
  	pinMode(motLC, OUTPUT);
  	pinMode(motLA, OUTPUT);  

    // defining inputs
  	pinMode(M2, INPUT);
  	pinMode(L2, INPUT);
  	pinMode(L1, INPUT);
  	pinMode(M1, INPUT);
  	pinMode(R1, INPUT);
  	pinMode(R2, INPUT);
  
  	delay(200); // just a gap for conformation
}

// function to takes the speed of both motors and give output signal
void run(int l, int r){

	if (l>=0){ // l>=0 means forward means clockwise for left motor
		analogWrite( motLC, l);
		digitalWrite(motLA, LOW);
	}
	else{ // l<0 means reverse means anticlockwise for left motor
		digitalWrite(motLC, LOW);
		analogWrite( motLA, -l);
	}

	if (r>0){  // r>=0 means forward means anticlockwise for right motor
		analogWrite( motRA, r);
		digitalWrite(motRC, LOW);
	}
	else{  // r>=0 means reverse means clockwise for right motor
		digitalWrite(motRA, LOW);
		analogWrite( motRC, -r);
	}
}

void loop() {
	// if track changes contrast then too this part helps to redifine what line is
	if ((dr(M1)==0) && (dr(M2)==0) && (dr(L2)==1) && (dr(L1)==1) && (dr(R1)==1) && (dr(R2)==1))
		line=0;  // middle1 and middle2 on line(0) and others on background
	else if ((dr(M1)==1) && (dr(M2)==1) && (dr(L2)==0) && (dr(L1)==0) && (dr(R1)==0) && (dr(R2)==0))
		line=1;  // middle1 and middle2 on line(1) and others on background

	/* if line was broken and at exectly this spot the contrast was changed then what the middle2 see 
	is line and what the rest see is background (as only the middle2 is on line*/
	else if ((dr(M1)==0) && (dr(M2)==1) && (dr(L2)==0) && (dr(L1)==0) && (dr(R1)==0) && (dr(R2)==0))
		line=1;  // middle2 on line(1) and other on background due to line break
	else if ((dr(M1)==1) && (dr(M2)==0) && (dr(L2)==1) && (dr(L1)==1) && (dr(R1)==1) && (dr(R2)==1))
		line=0;  // middle2 on line(0) and other on background due to line break

	if ((dr(M1)==line) && (dr(M2)==line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(200, 200);  // forward in high speed

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(150, 150);  // speed reduced when middle2 sensor dont see line but middle1 does

	if ((dr(M1)!=line) && (dr(M2)==line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(150, 150);  // if line is broken then the middle2 will be the first to sense, hence speed reduced

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)==line) && (dr(L1)==line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(-100, 100);  // 90degree left 

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)==line) && (dr(R2)==line))
		run(100, -100);  // 90degree right

	if ((dr(M1)==line) && (dr(M2)==line) && (dr(L2)==line) && (dr(L1)==line) && (dr(R1)==line) && (dr(R2)==line))
		run(-100, 100);  // in case of + junction, go left

	if ((dr(M1)!=line) && (dr(M2)!=line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(-100, 100);  // in case of dead end takes a U-turn (rotates anticlockwise)

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)!=line) && (dr(L1)==line) && (dr(R1)!=line) && (dr(R2)!=line))
		run(-1, 150);  // soft left for obtuse left turns (-1 to completely stop left wheel)

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)!=line) && (dr(L1)!=line) && (dr(R1)==line) && (dr(R2)!=line))
		run(150, -1);  // soft right for obtuse right turns (-1 to completely stop right wheel)

	if ((dr(M1)==line) && (dr(M2)!=line) && (dr(L2)==line) && (dr(L1)==line) && (dr(R1)==line) && (dr(R2)==line))
		run(0, 0);  // stops at T-junction
}
