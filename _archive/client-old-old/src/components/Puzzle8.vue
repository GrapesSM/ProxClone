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
        <!-- title Docked Ship -->
        <text x="290" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">DOCKED SHIP</text>
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
        <!-- Status Board -->
        <circle cx="100" cy="100" r="25" fill="darkgreen" stroke="black" />
        <circle cx="170" cy="100" r="25" fill="olive" stroke="black" />
        <circle cx="240" cy="100" r="25" fill="darkred" stroke="black" />
        <circle cx="100" cy="170" r="25" fill="darkgreen" stroke="black" />
        <circle cx="170" cy="170" r="25" fill="olive" stroke="black" />
        <circle cx="240" cy="170" r="25" fill="darkred" stroke="black" />
        <circle cx="100" cy="240" r="25" fill="darkgreen" stroke="black" />
        <circle cx="170" cy="240" r="25" fill="olive" stroke="black" />
        <circle cx="240" cy="240" r="25" fill="darkred" stroke="black" />

        <!-- Syncro Key Status -->
        <!-- <text x="100" y= "330" style="fill: none; stroke: #000000;  font-size: 30px;">Syncro Key:</text>
                    <foreignObject x="260" y="310" width="200" height="150">
                        <input name="minPower" type="text" value="No Data"/>
        </foreignObject>-->

        <!-- Syncro Key -->
        <text x="880" y="105" style="fill: none; stroke: #000000;  font-size: 20px;">SYNCHRO KEY</text>
        <text x="740" y="140" style="fill: none; stroke: #000000;  font-size: 20px;">1----2----3</text>
        <circle cx="750" cy="100" r="15" fill="white" stroke="black" />
        <circle cx="790" cy="100" r="15" fill="white" stroke="black" />
        <circle cx="830" cy="100" r="15" fill="white" stroke="black" />
        <foreignObject x="870" y="130" width="200" height="150">
          <input name="txtSyncro" type="text" v-bind:value="state.registers[13]" />
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