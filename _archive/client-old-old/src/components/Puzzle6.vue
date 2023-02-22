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
        <!-- title Life Support -->
        <text x="200" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">Life Support</text>

        <!-- Power Switch -->
        <rect
          id="power"
          x="40"
          y="20"
          width="30"
          height="30"
          v-bind:fill="['ENABLE', 'ON'].indexOf(stateNames[state.registers[6]]) != -1 ? 'white': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <text x="90" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>

        <!-- External Vent-->
        <text x="60" y="150" style="fill: none; stroke: #000000;  font-size: 25px;">External vent</text>
        <circle cx="280" cy="140" r="15" fill="black" stroke="white" />
        <foreignObject x="330" y="130" width="200" height="150">
          <input name="txtExternal" type="text" v-bind:value="stateNames[state.registers[9]]" />
        </foreignObject>

        <!-- Air Supply Pump-->
        <text x="60" y="200" style="fill: none; stroke: #000000;  font-size: 25px;">Air Supply Pump</text>
        <circle cx="280" cy="190" r="15" fill="black" stroke="white" />
        <foreignObject x="330" y="180" width="200" height="150">
          <input name="txtAir" type="text" v-bind:value="stateNames[state.registers[8]]" />
        </foreignObject>

        <!-- Air Pressure Status-->
        <text
          x="60"
          y="250"
          style="fill: none; stroke: #000000;  font-size: 25px;"
        >Air Pressure Status</text>
        <rect
          id="redAirPressure"
          x="300"
          y="230"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[7]] == 'BALANCED' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="greenAirPressure"
          x="350"
          y="230"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[7]] == 'UNBALANCED' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />

        <!-- Air Pressure Display-->
        <text x="60" y="300" style="fill: none; stroke: #000000;  font-size: 25px;">Air Pressure</text>
        <foreignObject x="330" y="280" width="200" height="150">
          <input name="txtAirPressure" type="text" v-bind:value="state.registers[13]" />
        </foreignObject>
        <text x="500" y="300" style="fill: none; stroke: #000000;  font-size: 25px;">kPa</text>

        <!-- External Vent Status-->
        <text
          x="600"
          y="100"
          style="fill: none; stroke: #000000;  font-size: 25px;"
        >EXTERNAL VENT STATUS</text>
        <rect
          id="redExternal"
          x="920"
          y="80"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[9]] == 'OPEN' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="greenExternal"
          x="970"
          y="80"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[9]] == 'CLOSED' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />

        <!-- Air Suplly Status-->
        <text
          x="600"
          y="160"
          style="fill: none; stroke: #000000;  font-size: 25px;"
        >AIR SUPPLY STATUS</text>
        <rect
          id="redAirSupply"
          x="920"
          y="140"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[8]] == 'ON' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="greenAirSupply"
          x="970"
          y="140"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[8]] == 'OFF' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />

        <!-- Connection Status -->
        <text x="750" y="300" style="fill: none; stroke: #000000;  font-size: 30px;">Connection:</text>
        <foreignObject x="940" y="280" width="200" height="150">
          <input name="txtConnection" type="text" style="width:150px;" v-bind:value="state.registers[3] > 0 ? 'Connected': 'No Connection'" />
        </foreignObject>

        <!-- Puzzle Status -->
        <text x="750" y="360" style="fill: none; stroke: #000000;  font-size: 30px;">Puzzle Status:</text>
        <foreignObject x="940" y="340" width="200" height="150">
          <input name="txtStatus" type="text" style="width:150px;" v-bind:value="stateNames[state.registers[5]]" />
        </foreignObject>
      </svg>
    </form>
  </div>
</template>

<script>
import axios from "axios";

export default {
  props: ['id', 'name', 'keyName', 'state', 'stateNames'],
  data() {
    return {
      state: this.state
    }
  },
  methods: {
    getPuzzle() {
      const path = `http://${window.location.hostname}:5000/puzzles/` + this.id;
      axios
        .get(path)
        .then(res => {
          this.state = res.data.puzzle.state;
        })
        .catch(error => {
          // eslint-disable-next-line
          console.error(error);
        });
    }
  },
  created() {
    // this.getPuzzle();
    
    this.reloadPuzzle = setInterval(() => {
      this.getPuzzle();
    }, 3000);
  }
};
</script>