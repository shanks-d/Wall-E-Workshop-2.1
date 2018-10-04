#include "TUNING.h"

static EventGroupHandle_t wifi_event_group;

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    const int CONNECTED_BIT = BIT0;
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        printf("%s\n",ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void initialise_wifi(void)

{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );

}

void http_server_netconn_serve(struct netconn *conn,float *setpoint,float *pitchKp,float *pitchKd,float *pitchKi,float *yaw_kP,float *yaw_kD,float *yaw_kI)
{
  const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
  const static char http_index_hml[] = "<html><head><title></title></head><body><table border = 1; cellspacing=0; width=70%><tr><td bgcolor = #9A9594; height = 40px; colspan=2 ; align=center ><b><i>TUNING</i></b></td></tr><tr><td height=25px; bgcolor=#DAD7D7> <a href =\"a\">Increase Pitch Kp</a></td><td height=25px; bgcolor=#DAD7D7><a href =\"b\">Decrease Pitch Kp</a> </td></tr><tr><td height=25px><a href =\"c\">Increase Pitch Kd </a></td><td height=25px> <a href =\"d\">Decrease Pitch Kd </a></td></tr><tr><td height=25px; bgcolor=#DAD7D7> <a href =\"e\">Increase Pitch Ki </a></td><td height=25px; bgcolor=#DAD7D7> <a href =\"g\"> Decrease Pitch Ki </a></td></tr><tr><td height=25px> <a href =\"h\">Increase Yaw Kp </a></td><td height=25px> <a href =\"i\">Decrease Yaw Kp </a></td></tr><tr><td height=25px; bgcolor=#DAD7D7> <a href =\"j\">Increase Yaw Kd </a></td><td height=25px; bgcolor=#DAD7D7> <a href =\"k\">Decrease Yaw Kd </a></td></tr><tr><td height=25px> <a href =\"l\">Increase Yaw Ki </a></td><td height=25px> <a href =\"m\">Decrease Yaw Ki </a></td></tr><tr><td height=25px; bgcolor=#DAD7D7><a href =\"v\">Increase Setpoint</a></td><td height=25px; bgcolor=#DAD7D7><a href =\"w\">Decrease Setpoint</a></td></tr></table></body></html></body></html>";
  char pKp[10];
  char pKd[10];
  char pKi[10];
  char yKp[10];
  char yKd[10];
  char yKi[10];
  char setPoint[15];
  char enter[]="<br>";
  char pitchArr[]="Pitch Parameters";
  char PitchKpArr[]="Kp = ";
  char PitchKdArr[]="Kd = ";
  char PitchKiArr[]="Ki = ";
  char yawArr[]= "Yaw Parameters";
  char YawKpArr[]="Kp = ";
  char YawKdArr[]="Kd = ";
  char YawKiArr[]="Ki = ";
  char setpointArr[]="setpoint = ";

  struct netbuf *inbuf;
  char *buf;
  u16_t buflen;
  err_t err;

  err = netconn_recv(conn, &inbuf);

  if (err == ERR_OK) {
    netbuf_data(inbuf, (void**)&buf, &buflen);

    if (buflen>=5 &&
        buf[0]=='G' &&
        buf[1]=='E' &&
        buf[2]=='T' &&
        buf[3]==' ' &&
        buf[4]=='/' ) 
    {

      if(buf[5]=='a'){
          *pitchKp+=0.1;
       }
       if(buf[5]=='b'){
          *pitchKp-=0.1;
       }
       if(buf[5]=='c'){
          *pitchKd+=0.1;
       }
       if(buf[5]=='d'){
          *pitchKd-=0.1;
       }
       if(buf[5]=='e'){
          *pitchKi+=0.01;
       }
       if(buf[5]=='g'){
          *pitchKi-=0.01;
       }
       if(buf[5]=='h'){
          *yaw_kP+=0.1;
       }
       if(buf[5]=='i'){
          *yaw_kP-=0.1;
       }
       if(buf[5]=='j'){
          *yaw_kD+=0.1;
       }
       if(buf[5]=='k'){
          *yaw_kD-=0.1;
       }
       if(buf[5]=='l'){
          *yaw_kI+=0.01;
       }
       if(buf[5]=='m'){
          *yaw_kI-=0.01;
       }
       if(buf[5]=='v'){
          *setpoint +=0.5;
       }
       if(buf[5]=='w'){
          *setpoint-=0.5;
       }
       // if(buf[5]=='n'){
       //    *forward_angle+=0.5;
       // }
       // if(buf[5]=='o'){
       //    *forward_angle-=0.5;
       // }


      gcvt(*pitchKp,5,pKp);
      gcvt(*pitchKd,5,pKd);
      gcvt(*pitchKi,5,pKi);
      gcvt(*yaw_kP,5,yKp);
      gcvt(*yaw_kD,5,yKd);
      gcvt(*yaw_kI,5,yKi);
      gcvt(*setpoint,5,setPoint);
      // gcvt(*forward_angle,5,forwardAngle);
      
      

      netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);


      netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
      netconn_write(conn, pitchArr, sizeof(pitchArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, PitchKpArr, sizeof(PitchKpArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, pKp, sizeof(pKp)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, PitchKdArr, sizeof(PitchKdArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, pKd, sizeof(pKd)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, PitchKiArr, sizeof(PitchKiArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, pKi, sizeof(pKi)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, yawArr, sizeof(yawArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, YawKpArr, sizeof(YawKpArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, yKp, sizeof(yKp)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, YawKdArr, sizeof(YawKdArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, yKd, sizeof(yKd)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, YawKiArr, sizeof(YawKiArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, yKi, sizeof(yKi)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);
      netconn_write(conn, setpointArr, sizeof(setpointArr)-1 , NETCONN_NOCOPY);
      netconn_write(conn, setPoint, sizeof(setPoint)-1, NETCONN_NOCOPY);
      netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);   
      // netconn_write(conn, forwardAngleArr, sizeof(forwardAngleArr)-1 , NETCONN_NOCOPY);
      // netconn_write(conn, forwardAngle, sizeof(forwardAngle)-1, NETCONN_NOCOPY);
      // netconn_write(conn, enter, sizeof(enter)-1, NETCONN_NOCOPY);  
        
    }

  }

  netconn_close(conn);

  netbuf_delete(inbuf);
}


