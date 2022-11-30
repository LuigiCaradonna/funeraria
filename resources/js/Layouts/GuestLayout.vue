<script setup>
import Nav from '@/Components/Nav.vue';

defineProps({
    canLogin: Boolean,
    canRegister: Boolean
})
</script>

<template>
    <Nav :canLogin=canLogin :canRegister=canRegister />

    <!-- Page Heading -->
    <header class="bg-white shadow" v-if="$slots.header">
        <div class="max-w-7xl mx-auto py-6 px-4 sm:px-6 lg:px-8">
            <slot name="header" />
        </div>
    </header>

    <slot />
</template>
