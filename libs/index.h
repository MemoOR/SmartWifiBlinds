/*
 * @Filename: index.h
 * @Authors: Guillermo Ortega Romo
 * @Description: This file contains the html for the AP mode configuration
 *              webserver
*/

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
  <head>
    <title>Blinds configuration</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      .content {
        max-width: 500px;
        margin: auto;
	      justify-content: center;
      }
      .center_div {
        justify-content: center;
        text-align: center;
      }
    </style>

    <script>
      function reloadPage() {
        setTimeout(function(){ document.location.reload(false); }, 500);   
      }
      function submitMessage() {
        alert("Saved value");
        reloadPage(); 
      }
    </script>
  </head>

  <body>
    <div class="content">
      <form action="/get" target="hidden-form">
        <div class="center_div">
          Network name (current value: %i_ssid%): 
          <input type="text" name="inputSSID"><br>
          Network password (current value: %i_pswd%): 
          <input type="text" name="inputPSW"><br>
          <input type="submit" value="Submit" onclick="submitMessage()">
        </div>
      </form><br>
      
      <div class="center_div">
        <h2> Vertical Blind Manual Movement </h2>
        <h5> (saved value: %s_v_time%) </h5>
        <h5> (current value: %i_v_time%) </h5>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_front">Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_stop" onclick="reloadPage()">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_back">Back</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "submit_v_time" onclick="submitMessage()">submit</button>
        </div>
      </form><br>

      <div class="center_div">
        <h2> Horizontal Blind Manual Movement </h2>
        <h5> (saved value: %s_h_time%) </h5>
        <h5> (current value: %i_h_time%) </h5>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_front">Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_stop" onclick="reloadPage()">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name="h_back">Back</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name="submit_h_time" onclick="submitMessage()">submit</button>
        </div>
      </form><br>

      <iframe style="display:none" name="hidden-form"></iframe>
    </div>
  </body>
</html>
)=====";
