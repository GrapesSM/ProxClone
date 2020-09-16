<template>
  <div>
    <form action id="frm">
      <svg width="1100" height="400" fill="black"> -->
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

        <!-- Puzzle Statuses -->
        <text x="35" y="220" style="fill: none; stroke: #000000;  font-size: 25px;">Laser Grid</text>
        <rect
          id="b7"
          x="70"
          y="250"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[9]] == 'SOLVED' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b6"
          x="70"
          y="300"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[9]] == 'ENABLE' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <text x="180" y="220" style="fill: none; stroke: #000000;  font-size: 25px;">Life Support</text>
        <rect
          id="b5"
          x="220"
          y="250"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[8]] == 'SOLVED' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b4"
          x="220"
          y="300"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[8]] == 'ENABLE' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <text x="350" y="220" style="fill: none; stroke: #000000;  font-size: 25px;">Keypad</text>
        <rect
          id="b3"
          x="370"
          y="250"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[10]] == 'SOLVED' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b2"
          x="370"
          y="300"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[10]] == 'ENABLE' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <text x="470" y="220" style="fill: none; stroke: #000000;  font-size: 25px;">Docked Ship</text>
        <rect
          id="b1"
          x="520"
          y="250"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[7]] == 'SOLVED' ? 'green': 'gray'"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b1"
          x="520"
          y="300"
          width="40"
          height="40"
          v-bind:fill="stateNames[state.registers[7]] == 'ENABLE' ? 'red': 'gray'"
          stroke="black"
          stroke-width="1"
        />

        <!-- Supply and Demand Status -->
        <text x="150" y="100" style="fill: none; stroke: #000000;  font-size: 30px;">Count Down</text>
        <foreignObject x="150" y="130" width="200" height="150">
          <input name="txtSupply" type="text" v-bind:value="state.registers[12]" />
        </foreignObject>

        <text x="500" y="100" style="fill: none; stroke: #000000;  font-size: 30px;">Key</text>
        <foreignObject x="500" y="130" width="200" height="150">
          <input name="txtDemand" type="text" v-bind:value="stateNames[state.registers[6]]" />
        </foreignObject>

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