import Vue from 'vue';
import Router from 'vue-router';
import Dashboard from './components/Dashboard.vue';
import Books from './components/Books.vue';
import Ping from './components/Ping.vue';

Vue.use(Router);

export default new Router({
  mode: 'history',
  base: process.env.BASE_URL,
  routes: [
    {
      path: '/books',
      name: 'Books',
      component: Books,
    },
    {
      path: '/ping',
      name: 'Ping',
      component: Ping,
    },
    {
      path: '/',
      name: 'Proxima - Dashboard',
      component: Dashboard,
    },
  ],
});