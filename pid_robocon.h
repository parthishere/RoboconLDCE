#define DESIRE_VALUE 0
#define kp 100
#define kd 100
#define ki 100
#define milltime_for_one_count 0.0000125
#define total_milltime_for_overflow 8.190;
#define SOMTHING 3

long int millsec       = 0;
long int Time          = 0;
long int elapsed_time  = 0;
long int prevTime      = 0;
long int overFlow      = 0;

int read_value    = 0;
int prev_Error    = 0;
int Error         = 0;
int pid_p         = 0;
int pid_d         = 0;
int pid_i         = 0;
int PID_value     = 0;

void PID_init(int sensor_nu_reading);
void PID_init_arduino(int sensor_nu_reading);
void start_counting_millsec();


void PID_init(int sensor_nu_reading)
{
    start_counting_millsec();
    Time = millsec + TCNT5*milltime_for_one_count;
    read_value = sensor_nu_reading;

    Error = DESIRE_VALUE - read_value; 

    pid_p = kp * Error;
    if(-SOMTHING < Error < SOMTHING)
    {
        pid_i = pid_i + ki*Error;
    }

    prevTime = Time;
    Time = millsec + TCNT5*milltime_for_one_count;
    elapsed_time = (Time - prevTime)*1000;
    pid_d = kd*( (Error - prev_Error) / elapsed_time );

    PID_value = pid_p + pid_i + pid_d;
    
    if(PID_value < 0) { PID_value = 0; }
    if(PID_value > 255) { PID_value = 255; } 

    prev_Error = Error;
}



void PID_init_arduino(int sensor_nu_reading)
{
    Time = millis(); 
    
    read_value = sensor_nu_reading;

    Error = DESIRE_VALUE - read_value; 

    pid_p = kp * Error;
    if(-SOMTHING < Error < SOMTHING)
    {
        pid_i = pid_i + ki*Error;
    }

    prevTime = Time;
    Time = millis();
    elapsed_time = (Time - prevTime)/ 1000;
    pid_d = kd*( (Error - prev_Error) / elapsed_time );

    PID_value = pid_p + pid_i + pid_d;
    
    if(PID_value < 0) { PID_value = 0; }
    if(PID_value > 255) { PID_value = 255; } 

    prev_Error = Error;
}







void start_counting_millsec()
{

  TCNT5 = 0;
  TCCR5B = 0;
  TCCR5A = (0 << WGM50);  // NORMAL MODE 
  TIMSK5 = (1 << TOIE5); // INTURRUPT ENABLE 

  TCCR5B = (1 << CS50); // TIMER COUNTER START

}

ISR (TIMER5_OVF_vect)
{
  overFlow++;
  millsec = millsec + overFlow*total_milltime_for_overflow;
}
