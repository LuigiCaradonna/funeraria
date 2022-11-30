<template>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
      <div class="container">
          <NavLink class="navbar-brand" href="/">
              <img src="https://funeraria.test/img/cmd-logo-nav.png" alt="" height="24">
          </NavLink>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarSupportedContent">
          <ul class="navbar-nav mx-auto mb-2 mb-lg-0">
              <li class="nav-item">
                  <NavLink v-if="$page.props.auth.user" :active="$page.component === 'Tombs'" aria-current="page" :href="route('tombs')">Lapidi</NavLink>
              </li>
          </ul>
          <div v-if="canLogin && !$page.props.auth.user" class="d-flex">
            <ul class="navbar-nav">
                <li class="nav-item">
                    <NavLink :active="$page.component === 'Login'" aria-current="page" href="/login">Log In</NavLink>
                </li>
                <li class="nav-item">
                    <NavLink v-if="canRegister" :active="$page.component === 'Register'" aria-current="page" href="/register">Register</NavLink>
                </li>
            </ul>
          </div>
        </div>
      </div>
    </nav>
</template>

<script setup>
import NavLink from './NavLink.vue';

defineProps ({
    canLogin: Boolean,
    canRegister: Boolean
});
</script>