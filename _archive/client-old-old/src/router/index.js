import Vue from 'vue';
import Router from 'vue-router';
import Ping from '../components/Ping.vue';
import Dashboard from '../components/Dashboard.vue';
import store from '../store';




export default new Router({
  mode: 'history',
  routes: [
    {
      path: '/ping',
      name: 'Ping',
      component: Ping,
    },
    {
      path: '/',
      name: 'Proxima - Dashboard',
      component: Dashboard,
    }
  ]
});
