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

      function disableButtons(clicked_id){
        if(clicked_id != 'v_f_b'){
          document.getElementById('v_f_b').disabled = true;
        }        
        if(clicked_id != 'v_b_b'){
          document.getElementById('v_b_b').disabled = true;
        }
        if(clicked_id != 'h_f_b'){
          document.getElementById('h_f_b').disabled = true;
        }
        if(clicked_id != 'h_b_b'){
          document.getElementById('h_b_b').disabled = true;
        }

        document.getElementById('v_s_b').disabled = true;
        document.getElementById('h_s_b').disabled = true;
        document.getElementById('wifi_sub').disabled = true;
        document.getElementById('v_sub').disabled = true;
        document.getElementById('h_sub').disabled = true;

        if(clicked_id === 'v_f_b' || clicked_id === 'v_b_b'){
          document.getElementById('v_s_b').removeAttribute('disabled');
        }
        if(clicked_id === 'h_f_b' || clicked_id === 'h_b_b'){
          document.getElementById('h_s_b').removeAttribute('disabled');
        }
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
          <input type="submit" id = "wifi_sub" value="Submit" onclick="submitMessage()">
        </div>
      </form><br>
      
      <div class="center_div">
        <h2> Vertical Blind Manual Movement </h2>
        <h5> (saved value: %s_v_time%) </h5>
        <h5> (current value: %i_v_time%) </h5>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_front" id = "v_f_b" onclick="disableButtons(this.id)" >Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_stop" id = "v_s_b" onclick="reloadPage()">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "v_back" id = "v_b_b" onclick="disableButtons(this.id)">Back</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "submit_v_time" id = "v_sub" onclick="submitMessage()">submit</button>
        </div>
      </form><br>

      <div class="center_div">
        <h2> Horizontal Blind Manual Movement </h2>
        <h5> (saved value: %s_h_time%) </h5>
        <h5> (current value: %i_h_time%) </h5>
      </div>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_front" id = "h_f_b" onclick="disableButtons(this.id)">Front</button>
        </div>
      </form><br>
      
      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name = "h_stop" id = "h_s_b" onclick="reloadPage()">Stop</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name="h_back" id = "h_b_b" onclick="disableButtons(this.id)">Back</button>
        </div>
      </form><br>

      <form action="/get" target="hidden-form">
        <div class="center_div">
          <button name="submit_h_time" id = "h_sub" onclick="submitMessage()">submit</button>
        </div>
      </form><br>

      <iframe style="display:none" name="hidden-form"></iframe>
    </div>
  </body>
</html>
)=====";
