<template>
  <div>
    <form action id="frm">
      <svg width="1100" height="400" fill="black">
        <!-- Background -->
        <rect
          x="0"
          y="0"
          width="1100"
          height="400"
          fill="LightGray"
          stroke="black"
          stroke-width="1"
        />
        <!-- Syncro Key -->
        <text x="180" y="305" style="fill: none; stroke: #000000;  font-size: 20px;">SYNCHRO KEY</text>
        <text x="40" y="340" style="fill: none; stroke: #000000;  font-size: 20px;">1----2----3</text>
        <circle cx="50" cy="300" r="15" fill="white" stroke="black" />
        <circle cx="90" cy="300" r="15" fill="white" stroke="black" />
        <circle cx="130" cy="300" r="15" fill="white" stroke="black" />
        <foreignObject x="180" y="320" width="200" height="150">
          <input name="txtSyncro" type="text" value="No Data" />
        </foreignObject>

        <!-- Baterry Matrix -->
        <text x="610" y="120" style="fill: none; stroke: #000000;  font-size: 30px;">Baterry Matrix</text>
        <foreignObject x="850" y="100" width="200" height="150">
          <input name="txtBatteryMatrix" type="text" value="No Data" />
        </foreignObject>
        <rect
          id="bc1"
          x="600"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc2"
          x="660"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc3"
          x="720"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc4"
          x="780"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc5"
          x="840"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc6"
          x="900"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc7"
          x="960"
          y="150"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <!-- Generator  -->
        <text x="610" y="270" style="fill: none; stroke: #000000;  font-size: 30px;">Generator</text>
        <foreignObject x="850" y="250" width="200" height="150">
          <input name="txtGenerator" type="text" value="No Data" />
        </foreignObject>
        <rect
          id="bc8"
          x="600"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc9"
          x="660"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc10"
          x="720"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc11"
          x="780"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc12"
          x="840"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc13"
          x="900"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="bc14"
          x="960"
          y="300"
          width="50"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <!-- Power Display -->
        <!-- <rect x="200" y= "130" width="100" height="70" fill="white" stroke = "black" stroke-width="1"/>
                    <rect x="210" y= "140" width="80" height="50" fill="white" stroke = "black" stroke-width="1"/>
        <text x="215" y= "175" style="fill: none; stroke: #000000;  font-size: 30px;">0000</text>-->
        <foreignObject x="250" y="180" width="200" height="150">
          <input name="txtPower" type="text" value="0" s />
        </foreignObject>
        <!-- Power Dial -->
        <circle cx="100" cy="170" r="50" fill="white" stroke="black" />
        <circle cx="100" cy="170" r="40" fill="white" stroke="black" />
        <!-- Power Switch left-->
        <rect
          id="powerLeft"
          x="40"
          y="20"
          width="30"
          height="30"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <text x="90" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>
        <!-- Power Switch right-->
        <rect
          id="powerRight"
          x="940"
          y="20"
          width="30"
          height="30"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <text x="990" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>
        <!-- Static Texts -->
        <text x="200" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">Docked Ship</text>
        <text
          x="200"
          y="105"
          style="fill: none; stroke: #000000;  font-size: 20px;"
        >ENERGY SUPLLEMENTAL</text>

        <!-- Color Picker
                    <foreignObject x="603" y="193" width="200" height="150">
                        <input id="c1" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="663" y="193" width="200" height="150">
                        <input id="c2" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="723" y="193" width="200" height="150">
                        <input id="c3" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="783" y="193" width="200" height="150">
                        <input id="c4" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="843" y="193" width="200" height="150">
                        <input id="c5" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="903" y="193" width="200" height="150">
                        <input id="c6" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="963" y="193" width="200" height="150">
                        <input id="c7" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="603" y="343" width="200" height="150">
                        <input id="c8" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>    
                    <foreignObject x="663" y="343" width="200" height="150">
                        <input id="c9" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="723" y="343" width="200" height="150">
                        <input id="c10" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="783" y="343" width="200" height="150">
                        <input id="c11" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="843" y="343" width="200" height="150">
                        <input id="c12" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="903" y="343" width="200" height="150">
                        <input id="c13" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
                    </foreignObject>
                    <foreignObject x="963" y="343" width="200" height="150">
                        <input id="c14" type="color" value="#ffffff" onchange="changeColor(this.id)"/>
        </foreignObject>-->
      </svg>
      <!-- Connection Status -->
      <text x="750" y="300" style="fill: none; stroke: #000000;  font-size: 30px;">Connection:</text>
      <input name="txtConnection" style="width:150px;" type="text" value="No Connection" />

      <!-- Puzzle Status -->
      <text x="750" y="360" style="fill: none; stroke: #000000;  font-size: 30px;">Puzzle Status:</text>
      <input name="txtStatus" style="width:150px;" type="text" value="No Data " />
    </form>
  </div>
</template>

<script>
export default {
  props: []
};
</script>