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
      function submitMessage() {
        alert("Saved value");
        setTimeout(function(){ document.location.reload(false); }, 500);   
      }
    </script>
  </head>

  <body>
    <div class="content">
      <form action="/get" target="hidden-form">
        <div class="center_div">
          Network name (current value: %i_ssid%): <input type="text" name="inputSSID"><br>
          <input type="submit" value="Submit" onclick="submitMessage()">
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          Network password (current value: %i_pswd%): <input type="text" name="inputPSW"><br>
          <input type="submit" value="Submit" onclick="submitMessage()">
        </div>
      </form><br>
      
      <div class="center_div">
        <h2> Vertical Blind Manual Movement </h2>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_front">Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_stop">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_back">Back</button>
        </div>
      </form><br>

      <div class="center_div">
        <h2> Horizontal Blind Manual Movement </h2>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_front">Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_stop">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_back">Back</button>
        </div>
      </form><br>

      <iframe style="display:none" name="hidden-form"></iframe>
    </div>
  </body>
</html>
)=====";
