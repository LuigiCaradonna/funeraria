import './bootstrap';
import '../css/app.css';

import { createApp, h } from 'vue';
import { createInertiaApp, Head, Link } from '@inertiajs/inertia-vue3';
import { InertiaProgress } from '@inertiajs/progress';
import { resolvePageComponent } from 'laravel-vite-plugin/inertia-helpers';
import { ZiggyVue } from '../../vendor/tightenco/ziggy/dist/vue.m';
import AuthenticatedLayout from './Layouts/AuthenticatedLayout.vue';
import VueSweetalert2 from 'vue-sweetalert2';
import 'sweetalert2/dist/sweetalert2.min.css';

const appName = window.document.getElementsByTagName('title')[0]?.innerText || 'Laravel';

createInertiaApp({
    title: (title) => `${title} - ${appName}`,
    resolve: async name => {

        let page = (await resolvePageComponent(`./Pages/${name}.vue`, import.meta.glob('./Pages/**/*.vue'))).default;
    
        // Use the specific layout for the component or the default one if a specific one is not set
        if (page.layout === undefined) {
          page.layout = AuthenticatedLayout;
        }
    
        return page;
    },
    setup({ el, app, props, plugin }) {
        return createApp({ render: () => h(app, props) })
            .use(plugin)
            .use(ZiggyVue, Ziggy)
            .use(VueSweetalert2)
            .component("Link", Link) // Globally registered components, no need to add inside vue components
            .component("Head", Head) 
            .mount(el);
    },
});

// Progress bar for page loading
InertiaProgress.init({
    // The delay after which the progress bar will
    // appear during navigation, in milliseconds.
    // delay: 250,
  
    // The color of the progress bar.
    color: '#F00',
  
    // Whether to include the default NProgress styles.
    // includeCSS: true,
  
    // Whether the NProgress spinner will be shown.
    // showSpinner: false,
  });
