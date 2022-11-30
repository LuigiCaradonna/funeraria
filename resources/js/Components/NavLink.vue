<template>
    <Link 
        class="nav-link" 
        :class="{'active' : active}" 
        aria-current="page"
    >
        <slot />
    </Link>
</template>

<script setup>
defineProps({
    active: Boolean
});
</script>