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
        <!-- title Laser Grid -->
        <text x="690" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">LASER GRID</text>
        <!-- Power Switch -->
        <rect
          id="power"
          x="40"
          y="20"
          width="30"
          height="30"
          v-bind:fill="stateNames[state.registers[6]] == 'ON' ? 'white': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <text x="90" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>

        <!-- Laser Grid  -->
        <text x="230" y="120" style="fill: none; stroke: #000000;  font-size: 30px;">LASER GRID</text>
        <rect
          x="160"
          y="90"
          width="340"
          height="100"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="230" y="150" width="200" height="150">
          <input name="txtLaserGrid" type="text" v-bind:value="stateNames[state.registers[8]]" />
        </foreignObject>
        <!-- Key left  -->
        <text x="110" y="240" style="fill: none; stroke: #000000;  font-size: 30px;">Key Left</text>
        <rect
          x="150"
          y="280"
          width="30"
          height="100"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="80" y="250" width="200" height="150">
          <input name="txtKeyLeft" type="text" v-bind:value="stateNames[state.registers[7]]" />
        </foreignObject>
        <!-- Key right  -->
        <text x="460" y="240" style="fill: none; stroke: #000000;  font-size: 30px;">Key Right</text>
        <rect
          x="500"
          y="280"
          width="30"
          height="100"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="430" y="250" width="200" height="150">
          <input name="txtKeyRight" type="text" v-bind:value="stateNames[state.registers[7]]" />
        </foreignObject>
        <!-- Key below  -->
        <text x="280" y="310" style="fill: none; stroke: #000000;  font-size: 30px;">Key Below</text>
        <rect
          x="290"
          y="350"
          width="100"
          height="30"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="260" y="320" width="200" height="150">
          <input name="txtKeyRight" type="text" v-bind:value="stateNames[state.registers[7]]" />
        </foreignObject>

        <!-- Connection Status -->
        <text x="750" y="300" style="fill: none; stroke: #000000;  font-size: 30px;">Connection:</text>
        <foreignObject x="940" y="280" width="200" height="150">
          <input name="txtConnection" style="width:150px;" type="text" v-bind:value="state.registers[3] > 0 ? 'Connected': 'No Connection'" />
        </foreignObject>

        <!-- Puzzle Status -->
        <text x="750" y="360" style="fill: none; stroke: #000000;  font-size: 30px;">Puzzle Status:</text>
        <foreignObject x="940" y="340" width="200" height="150">
          <input name="txtStatus" style="width:150px;" type="text" v-bind:value="stateNames[state.registers[5]]" />
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
      const path = "http://localhost:5000/puzzles/" + this.id;
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