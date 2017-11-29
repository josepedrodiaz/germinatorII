
//######################################## SERIALCHECK ^^^^  #################################
//############################################ UPTIME vvvvv  #################################
String getUptime()
{
 long days=0;
 long hours=0;
 long mins=0;
 long secs=0;
 
 secs = millis()/1000; //convect milliseconds to seconds
 mins=secs/60; //convert seconds to minutes
 hours=mins/60; //convert minutes to hours
 days=hours/24; //convert hours to days
 secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max 
 mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
 hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
 
 //Display results
 String response = "";
   if (days>0) // days will displayed only if value is greater than zero
 {
   response += days;
   response += " day/s and ";
 }
 
 if(hours < 10){
   response += "0";
   response += hours;
 }else{
   response += hours;
   }
 response += ":";
 
 if(mins < 10){
   response += "0";
   response += mins;
 }else{
   response += mins;
   }
 response += ":";
 
 if(secs < 10){
   response += "0";
   response += secs;
 }else{
   response += secs;
   }
 
 return response;
}
//############################################ UPTIME ^^^^  #################################
