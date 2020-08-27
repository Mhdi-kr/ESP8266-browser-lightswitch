String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
 <style>
  body {font-size:140%;} 
  #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
  h2 {text-align:center; } 
  #LED_button { padding:10px 10px 10px 10px; width:100%;  background-color: #50FF50; font-size: 120%;}
 </style>
 
<script>
   function switchLED() 
  {
       var button_text = document.getElementById("LED_button").value;
     if (button_text=="Turn on the LED")
     {
       document.getElementById("LED_button").value = "Turn off the LED";
       ajaxLoad('LEDON'); 
     }
     else
     {
       document.getElementById("LED_button").value = "Turn on the LED";
       ajaxLoad('LEDOFF');
     }
  }
 
var ajaxRequest = null;
if (window.XMLHttpRequest)  { ajaxRequest =new XMLHttpRequest(); }
else                        { ajaxRequest =new ActiveXObject("Microsoft.XMLHTTP"); }
 
 
function ajaxLoad(ajaxURL)
{
  if(!ajaxRequest){ alert("AJAX is not supported."); return; }
 
  ajaxRequest.open("GET",ajaxURL,true);
  ajaxRequest.onreadystatechange = function()
  {
    if(ajaxRequest.readyState == 4 && ajaxRequest.status==200)
    {
      var ajaxResult = ajaxRequest.responseText;
    }
  }
  ajaxRequest.send();
}
 
</script>
 
 <title>LED Control</title>
</head>
<body>
 <div id='main'>
  <h2>LED Control</h2>
)====="; 
 
String html_2 = R"=====(  
  <input type="button" id = "LED_button" onclick="switchLED()" value="Turn on the LED"       /> 
)=====";
 
String html_4 = R"=====(    
 </div>
</body>
</html>
)====="; 
 
#include <ESP8266WiFi.h>
 
// change these values to match your network
char ssid[] = "Mahdi";       //  your network SSID (name)
char pass[] = "12345678";          //  your network password
 
WiFiServer server(80);
 
 
String request = "";
#define LED LED_BUILTIN
 
 
void setup() 
{
    pinMode(LED, OUTPUT);     
 
    Serial.begin(115200);
    Serial.println();
    Serial.println("Serial started at 115200");
    Serial.println("ESP8266_LED_CONTROL_AJAX_02");
    Serial.println();
 
    // Connect to a WiFi network
    Serial.print(F("Connecting to "));  Serial.println(ssid);
    WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }
 
    Serial.println("");
    Serial.println(F("[CONNECTED]"));
    Serial.print("[IP ");              
    Serial.print(WiFi.localIP()); 
    Serial.println("]");
 
    // start a server
    server.begin();
    Serial.println("Server started");
 
} // void setup()
 
 
 
void loop() 
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request); 
 
    if       ( request.indexOf("LEDON") > 0 )  { digitalWrite(LED, HIGH);  }
    else if  ( request.indexOf("LEDOFF") > 0 ) { digitalWrite(LED, LOW);   }
    else
    {
        boolean pinStatus = digitalRead(LED);
        if (pinStatus==HIGH) { html_2.replace("Turn on the LED","Turn off the LED");   }
        else                 { html_2.replace("Turn off the LED","Turn on the LED");   }
 
        client.flush();
        client.print( header );
        client.print( html_1 );   
        client.print( html_2 ); 
        client.print( html_4 ); 
        delay(5);
    }
 
 
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
}
