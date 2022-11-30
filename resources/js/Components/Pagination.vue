<template>
    <nav aria-label="Page navigation">
        <ul class="pagination justify-content-center">
            <li v-for="link in links" class="page-item">
                <Component
                    :is="link.url ? 'Link' : 'span'"
                    :href="link.url"
                    :class="{ active: link.active }"
                    class="page-link"
                    v-html="link.label"
                />
            </li>
        </ul>
    </nav>
</template>

<script>
export default {
    props: {
        links: Array
    }
}
</script>