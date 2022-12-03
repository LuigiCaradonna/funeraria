<script setup>
import { ref, inject, watch } from 'vue';
import { Inertia } from '@inertiajs/inertia';
import { useForm } from '@inertiajs/inertia-vue3';
import Pagination from '../Components/Pagination.vue';
import throttle from "lodash/throttle";

// SweetAlert
const swal = inject('$swal');

let props = defineProps({
    colors: Object,
    filters: Object
});

let new_entry = ref(true);
let field_to_order = ref('name');
let list_order = ref(true);
let search = ref(props.filters.search);

watch(search, throttle(function(value) {
    Inertia.get('/colors', {search: value, field: field_to_order, order: list_order}, {
        preserveState: true,
        replace: true 
    });
}, 300));

let formCU = useForm({
    id: 0,
    name: ''
});

let formD = useForm({
    ids: []
});

let submitCU = () => {
    if (formCU.hasErrors) { formCU.clearErrors(); }

    if ( new_entry )
        formCU.post('/colors/create', {
            onSuccess: () => {
                formCU.reset();
                document.getElementById('btnDismissModal').click();
            },
        });
    else
        formCU.patch('colors/' + formCU.id, {
            onSuccess: () => {
                formCU.reset();
                document.getElementById('btnDismissModal').click();
            },
        });
};

let submitD = () => {
    swal.fire({
        title: 'Eliminazione multipla',
        text: 'Stai per eliminare ' + formD.ids.length + ' colori',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Elimina'
    }).then((result) => {
        if (result.isConfirmed) {
            formD.delete('/colors/multidel', {
            onSuccess: () => {
                formD.reset();
            },
        });
        }
    });
}

function deleteColor(a) {
    swal.fire({
        title: 'Stai per eliminare',
        text: a.name,
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Elimina'
    }).then((result) => {
        if (result.isConfirmed) {
            Inertia.delete('colors/' + a.id);
        }
    });
}

function editColor(a) {
    if (formCU.hasErrors) { formCU.clearErrors(); }

    new_entry = false;
    formCU.name = a.name;
    formCU.id = a.id;
}

function newColor() {
    if (formCU.hasErrors) { formCU.clearErrors(); }

    new_entry = true;
    formCU.name = '';
    formCU.id = 0;
}

function listOrder(field) {
    if ( field_to_order != field ) {
        field_to_order = field;
        list_order = true;
    }
    else {
        list_order = ! list_order;
    }

    Inertia.get('/colors', {search: search.value, field: field_to_order, order: list_order}, {
        preserveState: true,
        replace: true 
    });
}
</script>

<template>
    <Head title="Colori" />

    <h1 class="text-center">Colori</h1>
    <div class="d-flex justify-content-between">
        <div>
            <button @click="newColor" type="button" class="btn btn-primary" data-bs-toggle="modal" data-bs-target="#colorModal">
            Nuovo colore
            </button>
            <button v-if="formD.ids.length > 0" @click="submitD" :disabled="formD.processing" type="button" class="btn btn-danger ms-2">
            Elimina selezionati
            </button>
        </div>
        <div class="input-group mb-3  w-25">
            <span class="input-group-text" id="search-field">
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-search" viewBox="0 0 16 16">
                    <path d="M11.742 10.344a6.5 6.5 0 1 0-1.397 1.398h-.001c.03.04.062.078.098.115l3.85 3.85a1 1 0 0 0 1.415-1.414l-3.85-3.85a1.007 1.007 0 0 0-.115-.1zM12 6.5a5.5 5.5 0 1 1-11 0 5.5 5.5 0 0 1 11 0z"/>
                </svg>
            </span>
            <input  v-model="search" 
                    type="text" 
                    class="form-control" 
                    placeholder="Cerca" 
                    aria-label="Cerca" 
                    aria-describedby="search-field">
        </div>
    </div>
    <div class="d-flex justify-content-around">
        <form @submit.prevent="submitD">
            <table class="table table-striped w-auto">
                <thead>
                    <tr>
                        <th scope="col"></th>
                        <th scope="col" @click="listOrder('name')" class="sortable">Nome 
                            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-sort-down-alt" viewBox="0 0 16 16">
                                <path d="M3.5 3.5a.5.5 0 0 0-1 0v8.793l-1.146-1.147a.5.5 0 0 0-.708.708l2 1.999.007.007a.497.497 0 0 0 .7-.006l2-2a.5.5 0 0 0-.707-.708L3.5 12.293V3.5zm4 .5a.5.5 0 0 1 0-1h1a.5.5 0 0 1 0 1h-1zm0 3a.5.5 0 0 1 0-1h3a.5.5 0 0 1 0 1h-3zm0 3a.5.5 0 0 1 0-1h5a.5.5 0 0 1 0 1h-5zM7 12.5a.5.5 0 0 0 .5.5h7a.5.5 0 0 0 0-1h-7a.5.5 0 0 0-.5.5z"/>
                            </svg>
                        </th>
                        <th scope="col">Azioni</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="color in colors.data" :key="color.id">
                        <td><input type="checkbox" v-model="formD.ids" :value="color.id" /></td>
                        <td>{{ color.name }}</td>
                        <td>
                            <div class="d-flex justify-content-between">
                                <span role="button" data-bs-toggle="modal" data-bs-target="#colorModal" @click="editColor(color)">
                                    <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-pencil-fill" viewBox="0 0 16 16">
                                        <path d="M12.854.146a.5.5 0 0 0-.707 0L10.5 1.793 14.207 5.5l1.647-1.646a.5.5 0 0 0 0-.708l-3-3zm.646 6.061L9.793 2.5 3.293 9H3.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.207l6.5-6.5zm-7.468 7.468A.5.5 0 0 1 6 13.5V13h-.5a.5.5 0 0 1-.5-.5V12h-.5a.5.5 0 0 1-.5-.5V11h-.5a.5.5 0 0 1-.5-.5V10h-.5a.499.499 0 0 1-.175-.032l-.179.178a.5.5 0 0 0-.11.168l-2 5a.5.5 0 0 0 .65.65l5-2a.5.5 0 0 0 .168-.11l.178-.178z"/>
                                    </svg>
                                </span>
                                <span role="button" @click="deleteColor(color)">
                                    <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-trash3-fill" viewBox="0 0 16 16">
                                        <path d="M11 1.5v1h3.5a.5.5 0 0 1 0 1h-.538l-.853 10.66A2 2 0 0 1 11.115 16h-6.23a2 2 0 0 1-1.994-1.84L2.038 3.5H1.5a.5.5 0 0 1 0-1H5v-1A1.5 1.5 0 0 1 6.5 0h3A1.5 1.5 0 0 1 11 1.5Zm-5 0v1h4v-1a.5.5 0 0 0-.5-.5h-3a.5.5 0 0 0-.5.5ZM4.5 5.029l.5 8.5a.5.5 0 1 0 .998-.06l-.5-8.5a.5.5 0 1 0-.998.06Zm6.53-.528a.5.5 0 0 0-.528.47l-.5 8.5a.5.5 0 0 0 .998.058l.5-8.5a.5.5 0 0 0-.47-.528ZM8 4.5a.5.5 0 0 0-.5.5v8.5a.5.5 0 0 0 1 0V5a.5.5 0 0 0-.5-.5Z"/>
                                    </svg>
                                </span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
        </form>
    </div>

    <Pagination :links="colors.links" v-if="colors.links.length > 3"/>

    <div class="modal fade" id="colorModal" tabindex="-1" aria-labelledby="colorModalLabel" aria-hidden="true">
        <div class="modal-dialog modal-dialog-centered">
            <div class="modal-content">
                <div class="modal-header">
                    <h1 class="modal-title fs-5" id="colorModalLabel">Nuovo colore</h1>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <div class="modal-body">
                    <form @submit.prevent="submitCU">
                        <div class="mb-3">
                            <label for="name" class="form-label">Nome</label>
                            <input type="name" class="form-control" v-model="formCU.name" id="name">
                            <small v-if="formCU.errors.name" v-text="formCU.errors.name" id="nameHelp" class="text-danger"></small>
                        </div>
                        <input type="hidden" v-model="formCU.id" id="colorId" />
                    </form>
                </div>
                <div class="modal-footer d-flex justify-content-between">
                    <button type="button" id="btnDismissModal" class="btn btn-secondary" data-bs-dismiss="modal">Chiudi</button>
                    <button type="button" @click="submitCU" :disabled="formCU.processing" class="btn btn-primary px-5">Salva</button>
                </div>
            </div>
        </div>
    </div>
</template>
