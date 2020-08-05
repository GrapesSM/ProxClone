<template>
  <div>
    {{ id }}
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
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <text x="90" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>

        <!-- Battery Level -->
        <text x="90" y="110" style="fill: none; stroke: #000000;  font-size: 30px;">Baterry Level</text>
        <rect
          id="b7"
          x="40"
          y="80"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b6"
          x="40"
          y="120"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b5"
          x="40"
          y="160"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b4"
          x="40"
          y="200"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b3"
          x="40"
          y="240"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b2"
          x="40"
          y="280"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <rect
          id="b1"
          x="40"
          y="320"
          width="40"
          height="40"
          fill="white"
          stroke="black"
          stroke-width="1"
        />

        <!-- Supply and Demand Status -->
        <text x="300" y="100" style="fill: none; stroke: #000000;  font-size: 30px;">SUPPLY:</text>
        <foreignObject x="300" y="130" width="200" height="150">
          <input name="txtSupply" type="text" v-bind:value="state.registers[12]" />
        </foreignObject>

        <text x="500" y="100" style="fill: none; stroke: #000000;  font-size: 30px;">DEMAND:</text>
        <foreignObject x="500" y="130" width="200" height="150">
          <input name="txtDemand" type="text" v-bind:value="state.registers[12]" />
        </foreignObject>

        <!-- Power Control -->
        <text x="300" y="250" style="fill: none; stroke: #000000;  font-size: 30px;">POWER CONTROL</text>
        <foreignObject x="300" y="280" width="200" height="150">
          <input name="txtPower" type="text" value="No Data" />
        </foreignObject>

        <!-- Connection Status -->
        <text x="750" y="300" style="fill: none; stroke: #000000;  font-size: 30px;">Connection:</text>
        <foreignObject x="940" y="280" width="200" height="150">
          <input name="txtConnection" type="text" style="width:150px;" value="No Connection" />
        </foreignObject>

        <!-- Puzzle Status -->
        <text x="750" y="360" style="fill: none; stroke: #000000;  font-size: 30px;">Puzzle Status:</text>
        <foreignObject x="940" y="340" width="200" height="150">
          <input name="txtStatus" type="text" style="width:150px;" v-bind:value="state_names[state.registers[5]]" />
        </foreignObject>
      </svg>
    </form>
  </div>
</template>

<script>
import axios from "axios";

export default {
  props: ['id'],
  data() {
    return {
    }
  },
  methods: {
    getPuzzle() {
      const path = "http://localhost:5000/puzzles/" + this.id;
      axios
        .get(path)
        .then(res => {
          this.name = res.data.puzzle.name;
          this.keyName = res.data.puzzle.key_name;
          this.state = res.data.puzzle.state;
          this.state_names = res.data.state_names;
        })
        .catch(error => {
          // eslint-disable-next-line
          console.error(error);
        });
    }
  },
  created() {
    this.getPuzzle();
    // this.reloadPuzzle = setInterval(() => {
    //   console.log(this.props.id)
    //   this.getPuzzle();

    // }, 3000);
  }
};
</script>