<script setup>

let props = defineProps({
    // Using an Object as type, not an Array, to support the Laravel pagination
    tombs: Object
})
</script>

<template>
    <Head title="Lapidi" />

    <h1>LAPIDI</h1>
</template>
