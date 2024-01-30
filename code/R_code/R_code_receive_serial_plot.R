
# instructions on how to use this script with this arduino sketch:
# 1) connect datalogger to computer via USB port
# 2) open arduiono sketch in Arduino IDE: /path_to_arduino_sketch/arduino_sketch_send_to_serial.ino
# 3) In Arduino IDE, select USB port datalogger (arduino) is attached (Tools -> Port) 
# 4) Load sketch on Arduino board, if not already done
# 5) at line number 19 below, change "port" to port used to connect the Arduino (step #3 above)
# 5) run this script

setwd("/path_to_where_you_want_data_to_be_saved/data") 

# load data & libaries
library(serial)

# for continous data collection
# open serial communications with arduino via tty
# need to use serial library for this to work
# make sure baud rate matches arduino code (e.g., 9600)
con <- serialConnection(name = "test_con", port = "ttyUSB0",
                        mode = "9600,n,8,1", buffering = "none",
                        newline = 1, translation = "cr")
open(con)

Sys.sleep(5)  # allow about 5 seconds to establish coms with arduino

# check to make sure script is working... should return data as a text string
# you might need to adjust Sys.sleep time (here) and/or delay in Arduino sketch (line 99 in "arduino_sketch_send_to_serial.ino") 
# to make sure data are not requested too frequently (faster than the Arduino can provide them)
write.serialConnection(con, "s"); Sys.sleep(6)  
text1 <- read.serialConnection(con); text1

woo_numeric <- c() # empty container to put voltage data
woo_time <- c()    # empty container to put time data

for(i in 1:2000000) {  # 86400 seconds in one day
  # requests data from arduino
  write.serialConnection(con, "s"); Sys.sleep(6)
  text1 <- read.serialConnection(con); text1
  
  # extract numeric characters only... add to numeric matrix
  text2 <- gsub("[^0-9. ]", "", text1); text2
  # get rid of first leading space
  text2 <- strsplit(text2, " "); text2
  text2 <- unlist(text2); text2
  text2 <- c(text2[1], text2[2], text2[3], text2[4])
  
  # add values into numeric matrix
  numeric_1 <- as.numeric(text2); numeric_1
  woo_numeric <- rbind(woo_numeric, numeric_1); woo_numeric
  
  # get time stamp  
  time <- as.POSIXct(Sys.time(), origin="1970-01-01", tz="UTC")
  woo_time <- rbind(woo_time, as.character(time)); woo_time
  # combine and rename columns
  df <- cbind.data.frame(woo_numeric, woo_time)
  colnames(df) <- c("sens1", "sens2", "sens3", "sens4", "date_time"); df
  
  # add a hour_fraction column to use as x axis (use lubridate package for less clunky solution)
  df$hour <- substr(df$date_time, 12, 13); head(df)
  df$min <- substr(df$date_time, 15, 16); head(df)
  df$min_frac <- as.numeric(df$min)/60; head(df) 
  df$hour_frac <- as.numeric(df$hour) + df$min_frac; head(df)
  df$date_time <- as.POSIXct(df$date_time)
  
  # plot data
  par(mfrow=c(2,2),  oma=c(1, 1, 1, 1), mai=c(.3, .4, .1, .4), mgp=c(1.3,0.5,0))
  if(i>2) {

    plot(df$sens1 ~ df$date_time, pch=21, bg="black", cex=0.2)
    legend("topright", "sens_1", pch=21, pt.bg="black")
    
    plot(df$sens2 ~ df$date_time, pch=21, bg="blue", cex=0.2)
    legend("topright", "sens_2", pch=21, pt.bg="blue")
    
    plot(df$sens3 ~ df$date_time, pch=21, bg="red", cex=0.2)
    legend("topright", "sens_3", pch=21, pt.bg="red")
    
    plot(df$sens4 ~ df$date_time, pch=21, bg="green", cex=0.2)
    legend("topright", "sens_4", pch=21, pt.bg="green")
  }
  
  # save df and print i to console
  write.csv(df, "crap2.csv", row.names = FALSE)
  print(i); print(tail(df)); flush.console()

  Sys.sleep(30)  # sample interval
}



#  close(con)





