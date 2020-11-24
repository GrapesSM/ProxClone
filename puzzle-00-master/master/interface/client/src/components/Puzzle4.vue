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

        <!-- Door  -->
        <text x="110" y="170" style="fill: none; stroke: #000000;  font-size: 30px;">DOOR</text>
        <rect
          x="40"
          y="130"
          width="220"
          height="220"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="90" y="190" width="100" height="150">
          <input name="txtCode" type="text" v-bind:value="stateNames[state.registers[9]]" />
        </foreignObject>

        <!-- Access Panel  -->
        <text x="350" y="260" style="fill: none; stroke: #000000;  font-size: 25px;">ACCESS PANEL</text>
        <rect
          x="340"
          y="200"
          width="180"
          height="170"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="370" y="280" width="100" height="150">
          <input name="txtCode" type="text" v-bind:value="stateNames[state.registers[7]]" />
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