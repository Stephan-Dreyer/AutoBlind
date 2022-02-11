const char ControlCenter[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
  <script>
    function show(shown, hidden) {
      document.getElementById(shown).style.display = 'block';
      document.getElementById(hidden).style.display = 'none';
      return false;
    }
  </script>
  <title>control pannel</title>

  <style type="text/css">
    html {
      background-color: #141414;
      color: rgb(255, 255, 255);
      font-family: Helvetica;
      text-decoration: none;
    }

    .buttons {
      color: rgb(255, 255, 255);
      position: relative;
      display: inline-block;
      height: 0;
      border-radius: 50%;
      background: #38a8e498;
      font-family: Helvetica, Arial Black, sans;
      font-size: 80px;
      text-align: center;
    }

    .buttons-abs {
      width: 35%;
      padding: 17.5% 0;
      margin-left: 10%;
      margin-top: 0px;
      line-height: 5px;
    }

    .buttons-switch {
      position: relative;
      width: 87%;
      padding-top: 5%;
      padding: 6.4%;
      border-radius: 0;
      margin: 0px;
      text-align: center;
      height: 100px;


    }

    .buttons-color {
      width: 24%;

      display: inline-block;
      padding-top: 12%;
      padding-bottom: 12%;
      background-color: #a1a1a15e;
      color: #ffffff;
      border-radius: 0;
      margin: 3px;
      text-align: center;
      font-size: 70px;

    }

    #header {
      width: 33%;
      display: inline-block;

      padding-top: 7%;
      padding-bottom: 7%;
      background-color: #a1a1a12f;
      color: #38a9e4;
      font-size: 130px;
      border-radius: 0;
      margin: 0px;
      margin-bottom: 10%;
      text-align: center;
    }



    #remote {
      background-color: #a1a1a12f;
      margin: 2%;

    }

    .buttons-bright {

      display: inline-block;
      width: 42%;
      padding: 33px;
      padding-top: 73px;
      padding-bottom: 73px;
      color: #38a9e4;
      ;
      border-radius: 0;
      border: 0;
      margin: 0px;
      text-align: center;
      height: 5%;
    }


    .buttons-reset {

      display: inline-block;
      width: 42%;
      padding: 33px;
      padding-top: 73px;
      padding-bottom: 73px;
      color: #38a9e4;
      ;
      border-radius: 0;
      border: 0;
      margin: 0px;
      text-align: center;
      height: 5%;
    }

    #alarmtimes {

      margin: auto;
      margin-top: 5%;
      background: #c9c9c938;
      border-radius: 0;
      width: 90%;
      height: 10%;

    }

    .buttons-alarm {
      width: 32%;

      display: inline-block;
      padding-top: 12%;
      padding-bottom: 12%;
      background-color: #a1a1a15e;
      color: #ffffff;
      border-radius: 0;
      margin: 4px;
      text-align: center;
      font-size: 70px;

    }

    @media only screen and (min-width: 1100px) {

      #remote {
        width: 40%;

        background-color: #a1a1a12f;
        margin: 2%;
      }

      .buttons-color {
        width: 22%;
        font-size: 30px;
        padding-top: 8%;
        padding-bottom: 8%;
      }

      #header {
        padding-top: 1%;
        padding-bottom: 1%;
        margin-bottom: 2%;
      }

      .buttons-abs {
        width: 15%;
        padding: 7.5% 0;
        margin-left: 8%;
        margin-top: 0px;
        line-height: 5px;
      }

      .buttons-switch {
        padding: 1.5%;
        width: 95.07%;
      }

      .buttons-reset {

        display: inline-block;
        width: 43%;
        padding: 3%;
      }

      .buttons-bright {

        display: inline-block;
        width: 43%;
        padding: 3%;
      }

      #alarmtimes {

        position: fixed;
        bottom: 600px;
        left: 800px;
        margin: auto;
        height: 210px;

        border-radius: 0;
        width: 50%;
      }

      .buttons-alarm {
        width: 15%;
        font-size: 50px;
      }
    }
  </style>
</head>

<body>
  <div id="Page1">
    <h1 id="header"><span id="tracker"> N/A</span> </h1>
    <h1 id="header"><span id="alarm_hour"> Zzz</span> </h1>
    <a href="sleep">
      <h1 id="header" style="background-color:rgba(255, 0, 0, 0.514); color:white;">Zzz</h1>
    </a>
    <a href="up_FULL">
      <h1 class="buttons buttons-abs">Open</h1>
    </a>
    <a href="down_FULL">
      <h1 class="buttons buttons-abs">Close</h1>
    </a>
    <a href="up">
      <h1 class="buttons buttons-abs">&uarr;</h1>
    </a>
    <a href="down">
      <h1 class="buttons buttons-abs">&darr;</h1>
    </a>
    <a href="#" onclick="return show('Page2','Page1');">
      <h1 class="buttons buttons-switch">LED </h1>
    </a>


    <a href="reset0">
      <h1 class="buttons buttons-reset" style="background-color:rgba(255, 255, 255, 0.5);">Reset Open</h1>
    </a>
    <a href="reset100">
      <h1 class="buttons buttons-reset" style="background-color:rgba(255, 255, 255, 0.2);">Reset Close</h1>
    </a>
  </div>
  <div id="Page2" style="display:none">
    <div id="remote">
      <a href="off">
        <h1 class="buttons buttons-color" id="off" style="background-color:rgba(255, 0, 0, 0.5);">Off</h1>
      </a>

      <a href="fade">
        <h1 class="buttons buttons-color" id="fade" style="background-color:black;">Fade</h1>
      </a>
      <a href="rave">
        <h1 class="buttons buttons-color" id="rave" style="background-color:black;">Rave</h1>
      </a>
      <a href="raid">
        <h1 class="buttons buttons-color" id="RGB" style="background-color:black;">Raid</h1>
      </a>
      <a href="white">
        <h1 class="buttons buttons-color" id="white" style="background-color:rgba(255, 255, 255, 0.5);"></h1>
      </a>
      <a href="orange">
        <h1 class="buttons buttons-color" id="Intruderalert" style="background-color:rgba(255, 128, 0, 0.5);"></h1>
      </a>
      <a href="red">
        <h1 class="buttons buttons-color" id="red" style="background-color:rgba(255, 0, 0, 0.5);"></h1>
      </a>
      <a href="green">
        <h1 class="buttons buttons-color" id="green" style="background-color:green;"></h1>
      </a>
      <a href="blue">
        <h1 class="buttons buttons-color" id="blue" style="background-color:rgba(0, 0, 255, 0.5);"></h1>
      </a>
      <a href="cyan">
        <h1 class="buttons buttons-color" id="cyan" style="background-color:rgba(0, 255, 255, 0.5);;"></h1>
      </a>
      <a href="magenta">
        <h1 class="buttons buttons-color" id="magenta" style="background-color:rgba(255, 0, 255, 0.5);"></h1>
      </a>
      <a href="yellow">
        <h1 class="buttons buttons-color" id="yellow" style="background-color:rgba(255, 255, 0, 0.5);"></h1>
      </a>
    </div>
    <a href="brightup">
      <h1 class="buttons buttons-bright" style="background-color:#bfbfbfb2;">Up</h1>
    </a>
    <a href="brightdown">
      <h1 class="buttons buttons-bright" style="background-color:#454545;">Down</h1>
    </a>
    <a href="#" onclick="return show('Page1','Page2');">
      <h1 class="buttons buttons-switch">BLIND</h1>
    </a>

    <div id="alarmtimes">

      <a href="630">
        <h1 class="buttons buttons-alarm">6:30</h1>
      </a>
      <a href="7">
        <h1 class="buttons buttons-alarm">7</h1>
      </a>
      <a href="8">
        <h1 class="buttons buttons-alarm">8</h1>
      </a>
      <a href="830">
        <h1 class="buttons buttons-alarm">8:30</h1>
      </a>
      <a href="9">
        <h1 class="buttons buttons-alarm">9</h1>
      </a>
      <a href="sleep">
        <h1 class="buttons buttons-alarm" style="background-color:rgba(255, 0, 0, 0.5);">Zzz</h1>
      </a>
    </div>

  </div>
  <script>

    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getalarm();
      getindicator();

    }, 1000); //2000mSeconds update rate

    function getindicator() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("tracker").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "indicator", true);

      xhttp.send();
    }

    function getalarm() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("alarm_hour").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "alarm", true);

      xhttp.send();
    }
  </script>
</body>

</html>
)=====";