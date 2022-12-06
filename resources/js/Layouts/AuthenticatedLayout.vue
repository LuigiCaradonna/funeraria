<script setup>
import FlashMessage from '@/Components/FlashMessage.vue';
import NavLink from '@/Components/NavLink.vue';

</script>

<template>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
        <div class="container">
            <NavLink class="navbar-brand" href="/">
                <img src="http://funeraria.test/img/cmd-logo-nav.png" alt="" height="24">
            </NavLink>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav mx-auto mb-2 mb-lg-0">
                    <li class="nav-item">
                        <NavLink :active="$page.component === 'Home'" aria-current="page" :href="route('home')">Home</NavLink>
                    </li>
                    <li class="nav-item">
                        <NavLink :active="$page.component === 'Tombs'" aria-current="page" :href="route('tombs')">Lapidi</NavLink>
                    </li>
                    <li class="nav-item">
                        <NavLink :active="$page.component === 'Clients'" aria-current="page" :href="route('clients')">Clienti</NavLink>
                    </li>
                    <li class="nav-item dropdown">
                        <NavLink class="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-bs-toggle="dropdown" aria-expanded="false">
                            Impostazioni
                        </NavLink>
                        <ul class="dropdown-menu" aria-labelledby="navbarDropdown">
                            <li><NavLink class="dropdown-item" :href="route('flames')">Fiamme</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('vases')">Vasi</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('lamps')">Lampade</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('sculptures')">Sculture</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('pictures')">Foto</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('outlines')">Righelli</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('decorations')">Decorazioni</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('materials')">Materiali</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('fonts')">Fonts</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('colors')">Colori</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('finishings')">Finiture</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('tomb_types')">Tipologie lapidi</NavLink></li>
                            <li><NavLink class="dropdown-item" :href="route('tomb_frames')">Cornici lapidi</NavLink></li>
                        </ul>
                    </li>
                </ul>
                <div class="d-flex">
                    <!-- Authentication Links -->
                    <ul v-if="$page.props.auth.user" class="navbar-nav">
                        <li class="nav-item dropdown">
                            <NavLink id="navbarDropdown" class="dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                {{ $page.props.auth.user.name }}
                            </NavLink>
                            <div class="dropdown-menu dropdown-menu-end" aria-labelledby="navbarDropdown">
                                <NavLink :active="$page.component === 'Logout'" method="post" as="button" href="/logout">Log Out</NavLink>
                            </div>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </nav>

    <div class="d-flex justify-content-around">
        <FlashMessage />
    </div>
    <!-- Page Content -->
    <slot />
</template>
