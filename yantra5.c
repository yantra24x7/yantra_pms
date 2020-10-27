#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <fwlib32.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#define LSIZ 100 
#define RSIZ 100

short mcPortNr = 8193;
IODBPSD mcParam;
ODBSYS info;
ODBST mcStatInfo;
ODBALMMSG mcAlarm[10];
ODBSPEED  mcspeed;
IODBTIME mctimer,mctimer1,mctimer2,mctimer3;
ODBSPLOAD  mcspindle[6];
ODBSVLOAD  mcaxis[6];
ODBDGN diag,diag1,diag2,diag3,diag4,diag5;
ODBDGN diag10,diag11,diag12,diag13,diag14;
char sp_temp [FILENAME_MAX],sv_temp [FILENAME_MAX];
struct odbpro prgnum;
IODBTIMER infor;
int tem[100]={0};
int flag1[100]={0};
int temp=0;
int flag;
int i,l,temp1=0,temp2=0,lastpart=0;
int s=0;
char buffer[150];
short p1=6711;
short p2=0;
short p3=8;
short rc;
unsigned short libHndl = 0;
int axno;
char machine_ip[20];
int job_flag=0;
char *prog_name;
char *prog_data=buffer;
unsigned long lin_len = 3;
unsigned long data_len = 100;
char buf[256];
char *program_name = buf;
char *subout;
char machine_ip[20];
short spindle=6;
short axis=6;
int program_flag = 0;
int type,hour,minute,second;
int Previous_status=-1;
char a[2]=",";
char base[2]="";
char s_q[2]="'";
char job_name[50];
char programe_number[20];
char parts_count [ FILENAME_MAX ],machine_status [ FILENAME_MAX ],feed_rate [ FILENAME_MAX ],cutting_speed [ FILENAME_MAX ],total_run_time [ FILENAME_MAX ];
char total_run_time [ FILENAME_MAX ],total_run_time_seconds [ FILENAME_MAX ],total_cutting_time [ FILENAME_MAX ],programe_numberr [ FILENAME_MAX ];
char total_cutting_time_second [ FILENAME_MAX ],run_time [ FILENAME_MAX ],run_time_seconds [ FILENAME_MAX ],machine_time [ FILENAME_MAX ];
int pn[100]={0};
int Backup_data(char *filename,char *content);
char line[RSIZ][LSIZ];
int tot = 0;

void Backup_dbdatalog(char *data);
void jobID();

int main(char **argv, int argc)
{
	CURL *apIFirst = curl_easy_init();
	CURL *apISecond = curl_easy_init();
	while(1)
    {
       	rc = cnc_startupprocess(3, "./fanuc.log");
	    for( l = 0; l < 1; l++)
		 {
		for (i = 0; i < 1 ; i++)
       	{
			
			FILE *fptr = NULL; 
		    int d = 0;
		 	fptr = fopen("machine_5", "r");
		    while(fgets(line[d], LSIZ, fptr)) 
		    {
		        line[d][strlen(line[d]) - 1] = '\0';	
		        d++;
		    }
		    tot = d;
		    //printf("\n ----------------------------------------------------------------------------------------------------------------");
			sprintf(machine_ip,"%s%s",base,line[i]);
			 rc = cnc_allclibhndl3( line[i], mcPortNr, 0,    &libHndl);
			if (rc == EW_OK)
			{
				cnc_sysinfo(libHndl, &info);
				axno = atoi(info.axes);
				rc = cnc_rdparam(libHndl, p1, p2, p3, &mcParam);
				if (rc != EW_OK)
					break;
				rc = cnc_statinfo(libHndl,&mcStatInfo);
				if (rc != EW_OK)
					break;
				rc = cnc_rdspeed(libHndl,-1,&mcspeed);
				if (rc != EW_OK)
					break;
				rc = cnc_rdtimer(libHndl,1,&mctimer1);
				if (rc != EW_OK)
					break;
				rc = cnc_rdtimer(libHndl,2,&mctimer2);
				if (rc != EW_OK)
					break;
				rc = cnc_rdtimer(libHndl,3,&mctimer3);
				if (rc != EW_OK)
					break;
				rc = cnc_rdtimer(libHndl,0,&mctimer);
				if (rc != EW_OK)
						break;
				rc = cnc_rdspmeter(libHndl,-1,&spindle,mcspindle);
				if (rc != EW_OK)
					break;
				rc = cnc_rdsvmeter(libHndl,&axis,mcaxis);
				if (rc != EW_OK)
					break;
				rc = cnc_rdprgnum(libHndl, &prgnum);
				if (rc != EW_OK)
					break;
                rc = cnc_gettimer(libHndl,&infor);
				infor.type = 1;
				if (rc != EW_OK)
					break;
       			program_flag=1;				
			}
			else
			{
				mcParam.u.rdata.prm_val=-1;
				mcStatInfo.run=100;
				job_flag = 0;
				mcStatInfo.alarm=0;
				mcStatInfo.emergency=0;
				program_flag=0;
				mcspeed.actf.data=0;
				mctimer.minute=0;
				mctimer.msec=0;
				mctimer1.minute=0;
				mctimer1.msec=0;
				mctimer2.minute=0;
				mctimer2.msec=0;
				mctimer3.minute=0;
				mctimer3.msec=0;
			}	
			
//--------------------------------------------------------spindle_load-------------------------------------------------			
		
		int l,dataa;
        for(l = 0 ; l < spindle ; l++) {
            
            //printf("\n****************spindleload*************************\n");
            //printf("%c%c = %d\n", mcspindle[l].spload.name, mcspindle[l].spload.suff1,mcspindle[l].spload.data);
			dataa=mcspindle[l].spload.data;
        }
//-------------------------------------------------------servo_load-----------------------------------------------------        
        int k;
        int data[10];

        for(k = 0 ; k < axis ; k++) 
        
        {
            
            char sv_loadd_name [ FILENAME_MAX ]; 
            char sv_loadd_suff1 [ FILENAME_MAX ];
            char sv_loadd_data [ FILENAME_MAX ];
            
           // printf("\n****************servoload*************************\n");
           // printf("%c%c = %d\n", mcaxis[k].svload.name, mcaxis[k].svload.suff1,mcaxis[k].svload.data);
            char sv_loadd_namee  =mcaxis[k].svload.name;
            int sv_loadd_suff11 =mcaxis[k].svload.suff1;
            int sv_loadd_dataa  =mcaxis[k].svload.data;
            snprintf(sv_loadd_name,FILENAME_MAX-1, "%s%c", base, sv_loadd_namee);
            data[k]=sv_loadd_dataa;
            //printf("\ndata=%d",data[k]);
            
			snprintf(sv_loadd_suff1,FILENAME_MAX-1, "%s%d", base, sv_loadd_suff11); 
			snprintf(sv_loadd_data,FILENAME_MAX-1, "%s%d", base, sv_loadd_dataa);
		  /*  printf("\nsv_loadd_name=%s\n",sv_loadd_name);
	     	printf("sv_loadd_suff1=%s\n",sv_loadd_suff1);
	        printf("sv_loadd_data=%s\n",sv_loadd_data);*/
			 
        }
        char sv [FILENAME_MAX];
        char sv_x [FILENAME_MAX];
        char sv_y [FILENAME_MAX];
        char sv_z [FILENAME_MAX];
		char sv_a [FILENAME_MAX];
		char sv_b [FILENAME_MAX];
        char sp [FILENAME_MAX];
      /*  sv_x=data[0];
        sv_y=data[1];
        sv_z=data[2];
        */
        snprintf(sv_x,FILENAME_MAX-1, "%d", data[0]);
        snprintf(sv_y,FILENAME_MAX-1, "%d", data[1]);
        snprintf(sv_z,FILENAME_MAX-1, "%d", data[2]);
		 snprintf(sv_a,FILENAME_MAX-1, "%d", data[3]);
		  snprintf(sv_b,FILENAME_MAX-1, "%d", data[4]);
       snprintf(sv,FILENAME_MAX-1, "X:%d,Y:%d,Z:%d,A:%d,B:%d", data[0],data[1],data[2],data[3],data[4]);
	  // printf("sv_LOADS=%s\n",sv);
       
       snprintf(sp,FILENAME_MAX-1, "%s%d", base,dataa);
      //   printf("\ndata=%s",sv);
     //  printf("\ndata=%s",sp);
	  
			int cutting_speedd = mcspeed.acts.data;
			char cutting_speed [ FILENAME_MAX ];
			snprintf(cutting_speed, FILENAME_MAX-1,"%s%d", base, cutting_speedd);
			
//-------------------------------------------Spindle_Temperature-------------------------------------------------------------------------                               
            
 				if(!rc) {

                 	int sp_tempp=diag.u.cdata;
					int sp_type=diag.type;
					 int sp_datano=diag.datano;
					
					//printf("type=%d\n",sp_type);
					  //printf("datano=%d\n",sp_datano);
					    //printf("sp_tempp=%d\n",sp_tempp);
					    snprintf(sp_temp,FILENAME_MAX-1, "%s%d", base, sp_tempp);
					    
                   }			 
                
//------------------------------------------Servo_temperature------------------------------------------------------------------
                     char sv_tempxx[10];
					 char sv_tempyy[10];
					  char sv_tempzz[10];
					  char sv_tempAA[10];
					  char sv_tempBB[10];                 
			
					
                    rc=cnc_diagnoss(libHndl, 308, 1 , 24, &diag1);  //plz add the vaild temp no
					
                    int sv_tempx=diag1.u.cdata;
					snprintf(sv_tempxx,FILENAME_MAX-1, "%d", sv_tempx);
                 // printf("sv_tempx=%d\n",sv_tempx);
                    rc=cnc_diagnoss(libHndl, 308, 2 , 24, &diag2);  //plz add the vaild temp no
					
                     int sv_tempy=diag2.u.cdata;
					 snprintf(sv_tempyy,FILENAME_MAX-1, "%d", sv_tempy);
					 
                   // printf("sv_tempy=%d\n",sv_tempy);
                      rc=cnc_diagnoss(libHndl, 308, 3 , 24, &diag3);  //plz add the vaild temp no
					 
                      int sv_tempz=diag3.u.cdata;
					  snprintf(sv_tempzz,FILENAME_MAX-1, "%d", sv_tempz);
					 
                     // printf("sv_tempz=%d\n",sv_tempz); 
					  
					   rc=cnc_diagnoss(libHndl, 308, 5 , 24, &diag4);  //plz add the vaild temp no
					 
                      int sv_tempA=diag4.u.cdata;
					  snprintf(sv_tempAA,FILENAME_MAX-1, "%d", sv_tempA);
					 
                      //printf("sv_tempAA=%s\n",sv_tempAA); 
					   rc=cnc_diagnoss(libHndl, 308, 4 , 24, &diag5);  //plz add the vaild temp no
					 
                      int sv_tempB=diag5.u.cdata;
					  snprintf(sv_tempBB,FILENAME_MAX-1, "%d", sv_tempB);
					 
                     // printf("sv_tempBB=%d\n",sv_tempB); 

//----------------------------------------------------------------------------------------------------------------------
			
			time_t rawtime;
			time (&rawtime);
			char machine[60] = "SELECT ID FROM MACHINES WHERE machine_ip =";
			int parts =  mcParam.u.rdata.prm_val;
			sprintf(parts_count, "%s%d", base, parts);
			int status =  mcStatInfo.run;
			s= mcStatInfo.run;
			//printf("sts=%d", s);
			int feed_ratee = mcspeed.actf.data;
			sprintf(feed_rate, "%s%d", base, feed_ratee);
			cutting_speedd = mcspeed.acts.data;
			sprintf(cutting_speed, "%s%d", base, cutting_speedd);
			int total_run_timee = mctimer1.minute;
			sprintf(total_run_time, "%s%d", base, total_run_timee);
			int total_run_time_secondss = mctimer1.msec;
			sprintf(total_run_time_seconds, "%s%d", base, total_run_time_secondss);
			int total_cutting_timee = mctimer2.minute;
			sprintf(total_cutting_time, "%s%d", base, total_cutting_timee);
			int total_cutting_time_secondss = mctimer2.msec;
			sprintf(total_cutting_time_second, "%s%d", base, total_cutting_time_secondss);
			int run_timee = mctimer3.minute;
			sprintf(run_time, "%s%d", base, run_timee);
			int run_time_secondss = mctimer3.msec;
			sprintf(run_time_seconds, "%s%d", base, run_time_secondss);
			int machine_time1 = infor.data.time.hour;
			int machine_time2 = infor.data.time.minute;
			int machine_time3 = infor.data.time.second;
			sprintf(machine_time, "%s%d:%d:%d", base, machine_time1,machine_time2,machine_time3);
			int programee_number = prgnum.mdata;                 
			if(status!=100 & status!=3)
			{
				programee_number = pn[i];
				sprintf(programe_numberr, "%s%d", base, programee_number);
			}
			else
			{
			if(program_flag == 1)
			{
			    if(programee_number != temp1)
			    {
					if(temp2 == parts)
				    {
						sprintf(programe_numberr, "%s%d", base, programee_number);
						lastpart = parts;
						parts =0;
						sprintf(parts_count, "%s%d", base, parts);
				    }
				    else
				    {
						sprintf(programe_numberr, "%s%d", base, programee_number);
						sprintf(parts_count, "%s%d", base, parts);
				    }
				}
				else
                {
				    if (parts == lastpart)
				    {
						sprintf(programe_numberr, "%s%d", base, programee_number);
						parts = 0;
						sprintf(parts_count, "%s%d", base, parts);
					}
                    else
                    {
						sprintf(programe_numberr, "%s%d", base, programee_number);
						sprintf(parts_count, "%s%d", base, parts);
                    }
                }
			}
			else
			{
				programee_number=0;
				strcpy(programe_numberr,"");
			}
			}
			if (job_flag == 1)
			{
				strcpy(job_name,subout);
				strcpy(subout,"");
			}
			else
			{
				strcpy(job_name,"");
			}
			sprintf(machine_status, "%s%d", base, status);
			strcat(machine,s_q);
			strcat(machine,machine_ip);
			strcat(machine,s_q);
			
			
			curl_easy_setopt(apIFirst, CURLOPT_URL,  "http://0.0.0.0:4002/api/v1/machines/api");
			char params[100] ="machine_status=";
			char params1[100]="&parts_count=";
			char params2[100]="&machine_id=";
			char params3[400]="&job_id=";
			char params4[100]="&created_at=";
			char params5[100]="&updated_at=";
			char params6[100]="&total_run_time=";
			char params61[100]="&total_run_time_seconds=";
			char params7[100]="&total_cutting_time=";
			char params71[100]="&total_cutting_time_second=";
			char params8[100]="&run_time=";
			char params81[100]="&run_time_seconds=";
			char params9[100]="&feed_rate=";
			char params10[100]="&cutting_speed=";
			char params11[100]="&programe_number=";
			char params12[100]="&machine_time=";
			char params13[100]="&sp=";
			char params14[100]="&sv=";
			char params15[100]="&sp_temp=";
			char params16[100]="&svtemp_x=";
			char params17[100]="&svtemp_y=";
			char params18[100]="&svtemp_z=";
			char params19[100]="&svtemp_a=";
			char params20[100]="&svtemp_b=";
			char params21[ 100 ]="&sv_x=";
			char params22[ 100 ]="&sv_y=";
			char params23[ 100 ]="&sv_z=";
			char params24[ 100 ]="&sv_a=";
			char params25[ 100 ]="&sv_b=";
			temp = tem[i];
			flag=flag1[i];			
			if(status==100)
			{
			strcat(params,machine_status);strcat(params,params1);strcat(params,"0");strcat(params,params2);strcat(params,machine_ip);
			strcat(params,params3);strcat(params,job_name);strcat(params,params4);strcat(params,ctime(&rawtime));strcat(params,params5);
			strcat(params,ctime(&rawtime));strcat(params,params6);strcat(params,total_run_time);strcat(params,params61);
			strcat(params,total_run_time_seconds);strcat(params,params7);strcat(params,total_cutting_time);strcat(params,params8);
			strcat(params,run_time);strcat(params,params71);strcat(params,total_cutting_time_second);strcat(params,params81);
			strcat(params,run_time_seconds);strcat(params,params9);strcat(params,feed_rate);strcat(params,params10);strcat(params,cutting_speed);
			strcat(params,params11);strcat(params,programe_numberr);strcat(params,params12);strcat(params,machine_time);
			strcat(params,params13);strcat(params,sp);
			strcat(params,params14);strcat(params,sv);
			strcat(params,params15);strcat(params,sp_temp);
			strcat(params,params16);strcat(params,sv_tempxx);
			strcat(params,params17);strcat(params,sv_tempyy);
			strcat(params,params18);strcat(params,sv_tempzz);
			//strcat(params,params19);strcat(params,sv_tempAA);
			//strcat(params,params20);strcat(params,sv_tempBB);
			strcat(params,params21);strcat(params,sv_x);
			strcat(params,params22);strcat(params,sv_y);
			strcat(params,params23);strcat(params,sv_z);
			//strcat(params,params24);strcat(params,sv_a);
			//strcat(params,params25);strcat(params,sv_b);
			tem[i] = parts;
			pn[i] = programee_number;
			}
			else if(temp!=parts)
			{
				strcat(params,machine_status);strcat(params,params1);strcat(params,parts_count);strcat(params,params2);strcat(params,machine_ip);
				strcat(params,params3);strcat(params,job_name);strcat(params,params4);strcat(params,ctime(&rawtime));strcat(params,params5);
				strcat(params,ctime(&rawtime));strcat(params,params6);strcat(params,total_run_time);strcat(params,params61);
				strcat(params,total_run_time_seconds);strcat(params,params7);strcat(params,total_cutting_time);strcat(params,params8);
				strcat(params,run_time);strcat(params,params71);strcat(params,total_cutting_time_second);strcat(params,params81);
				strcat(params,run_time_seconds);strcat(params,params9);strcat(params,feed_rate);strcat(params,params10);strcat(params,cutting_speed);
				strcat(params,params11);strcat(params,programe_numberr);strcat(params,params12);strcat(params,machine_time);
				strcat(params,params13);strcat(params,sp);
				strcat(params,params14);strcat(params,sv);
				strcat(params,params15);strcat(params,sp_temp);
				strcat(params,params16);strcat(params,sv_tempxx);
				strcat(params,params17);strcat(params,sv_tempyy);
				strcat(params,params18);strcat(params,sv_tempzz);
				//strcat(params,params19);strcat(params,sv_tempAA);
				//strcat(params,params20);strcat(params,sv_tempBB);
				strcat(params,params21);strcat(params,sv_x);
				strcat(params,params22);strcat(params,sv_y);
				strcat(params,params23);strcat(params,sv_z);
				//strcat(params,params24);strcat(params,sv_a);
				//strcat(params,params25);strcat(params,sv_b);
				tem[i] = parts;
				pn[i] = programee_number;
			}
			else
			{
				strcat(params,machine_status);strcat(params,params1);strcat(params,parts_count);strcat(params,params2);strcat(params,machine_ip);
				strcat(params,params3);strcat(params,job_name);strcat(params,params4);strcat(params,ctime(&rawtime));strcat(params,params5);
				strcat(params,ctime(&rawtime));strcat(params,params6);strcat(params,total_run_time);strcat(params,params61);
				strcat(params,total_run_time_seconds);strcat(params,params7);strcat(params,total_cutting_time);strcat(params,params8);
				strcat(params,run_time);strcat(params,params71);strcat(params,total_cutting_time_second);strcat(params,params81);
				strcat(params,run_time_seconds);strcat(params,params9);strcat(params,feed_rate);strcat(params,params10);strcat(params,cutting_speed);
				strcat(params,params11);strcat(params,programe_numberr);strcat(params,params12);strcat(params,machine_time);
				strcat(params,params13);strcat(params,sp);
				strcat(params,params14);strcat(params,sv);
				strcat(params,params15);strcat(params,sp_temp);
				strcat(params,params16);strcat(params,sv_tempxx);
				strcat(params,params17);strcat(params,sv_tempyy);
				strcat(params,params18);strcat(params,sv_tempzz);
				//strcat(params,params19);strcat(params,sv_tempAA);
				//strcat(params,params20);strcat(params,sv_tempBB);
				strcat(params,params21);strcat(params,sv_x);
				strcat(params,params22);strcat(params,sv_y);
				strcat(params,params23);strcat(params,sv_z);
				//strcat(params,params24);strcat(params,sv_a);
				//strcat(params,params25);strcat(params,sv_b);
				tem[i] = parts;
				pn[i] = programee_number;
			}

			/* if(flag!=0)
			{
			curl_easy_setopt(apIFirst, CURLOPT_CUSTOMREQUEST, "POST");
			curl_easy_setopt(apIFirst, CURLOPT_POSTFIELDS,params);
			printf("Test:%s",params);
			CURLcode res = curl_easy_perform(apIFirst);
			if(res != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
				continue;
			}
			}    */

			//printf("status=%d",s);
			//printf("Previous_status=%d\n",Previous_status);
			if((s!=Previous_status)||(Previous_status==-1))
					{
						curl_easy_setopt(apIFirst, CURLOPT_CUSTOMREQUEST, "POST");
						curl_easy_setopt(apIFirst, CURLOPT_POSTFIELDS,params);
						printf("Test:%s",params);
						//Backup_dbdatalog(params);
						CURLcode res = curl_easy_perform(apIFirst);
						if(res != CURLE_OK)
						{
							//fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
							continue;
						}
						printf("\n ----------------------------------------------------------------------------------------------------------------");
					}
			
			flag1[i]=1;
			strcpy(job_name,"");
			strcpy(programe_numberr,"");
			cnc_freelibhndl(libHndl);
			rc = cnc_exitprocess();
			//printf("\n rc<%d>/<%s>", rc, rc == EW_OK ?  "ok" : "flr");
			Previous_status=s;
		}
		 if(l=1);
		   l=-1;
		sleep(1);
		
		 }
	}
	return 0;
}

void jobID(char **argv, int argc)
{
	if (mcStatInfo.run == 3)
	{ 	
		if(prgnum.data == prgnum.mdata)
		{
			rc = cnc_exeprgname2(libHndl, program_name);
			if ( rc != EW_OK)
			{
				rc = cnc_pdf_rdmain(libHndl, program_name);
			}
			if(rc == EW_OK)  
			{
				rc = cnc_rdpdf_line(libHndl,program_name, 0, prog_data, &lin_len, &data_len);
				lin_len=3;
				data_len=100;
				if (rc == EW_OK)
				{
					if (strchr(prog_data,'O') != NULL)
					{
						job_flag = 1;
						const char *sub1 = strstr(prog_data, "O")+5;
						const char *sub2 = strstr(sub1, ")")+1;
						size_t len = sub2-sub1;
						subout = (char*)malloc(sizeof(char)*(len+1));
						strncpy(subout,sub1, len);
						subout[len] = '\0';
					}
					else
					{
						job_flag = 0;
						printf("No Job ID");
					}
				}
				else
				{
					job_flag = 0;
					printf("No Job ID");
				}
			}
			else
			{
				job_flag = 0;
				printf("No Job ID");
			}
		}
	}
	else
	{ 
		job_flag = 0;
		printf("\nManual mode");
	}
}


void get_time_info(char *timelog_val) //Function to get UTC in Y-M-D H:M in the address sent as paramater
{
	time_t rawtime;
 	struct tm * timeinfo_val;
  	time (&rawtime);
  	timeinfo_val = localtime (&rawtime);
	char dmy[26];
	strftime(dmy,26,"%Y-%m-%d %H:%M:%S",timeinfo_val);
		//printf("\n************\n%s\n*************\n",dmy);
  	sprintf (timelog_val,"%s",dmy);

}

int Backup_data(char *filename,char *content)
{
FILE *log=fopen(filename,"a");
char timeinfo[26];
get_time_info(timeinfo);
char *line = NULL;
 

			fprintf(log,"%s\n",content);
			fclose(log);

	return 0;			
}
void Backup_dbdatalog(char *data)
{
Backup_data("/home/cnc/Data_log/State_change.txt",data);

}









