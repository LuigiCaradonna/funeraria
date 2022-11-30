<script setup>
import { ref, inject, watch } from 'vue';
import { Inertia } from '@inertiajs/inertia';
import { Head, useForm } from '@inertiajs/inertia-vue3';
import Pagination from '../Components/Pagination.vue';

// debounce waits the given amount of time after the user stops typing before to send the request
// throttle sends a request every given amount of time even while the user is typing
import throttle from "lodash/throttle";

// SweetAlert
const swal = inject('$swal');

let props = defineProps({
    // Using an Object as type, not an Array, to support the Laravel pagination
    clients: Object,
    filters: Object
});

// Say whether to create a new user (true) or edit (false) an existing one
let new_entry = ref(true);

// Parameters to order the results

// Filed to order: default name
let field_to_order = ref('name');
// Ordering direction: true = ASC, false = DESC
let list_order = ref(true);

let search = ref(props.filters.search);

watch(search, throttle(function(value) {
    Inertia.get('/clients', {search: value, field: field_to_order, order: list_order}, {
        preserveState: true,
        // replaces the previous request avoiding to generate history entry for each keystroke
        // if not set, by clickng "back" on the browser it wouldn't go to the previous visited page,
        // but to the same page with the previous typed string in the search input
        replace: true 
    });
}, 300));

// Form to create or update a Client
let formCU = useForm({
    id: 0,
    name: '', 
    address: '', 
    email: '', 
    phone: ''
});

// Form to delete multiple Clients
let formD = useForm({
    ids: []
});

// Client's creation/update submit function
let submitCU = () => {
    if (formCU.hasErrors) { formCU.clearErrors(); }

    if ( new_entry )
        formCU.post('/clients/create', {
            onSuccess: () => {
                formCU.reset();
                document.getElementById('btnDismissModal').click();
            },
        });
    else
        formCU.patch('clients/' + formCU.id, {
            onSuccess: () => {
                formCU.reset();
                document.getElementById('btnDismissModal').click();
            },
        });
};

// Multiple Clients' delete submit function
let submitD = () => {
    // Popup an alert to confirm the deletion
    swal.fire({
        title: 'Eliminazione multipla',
        text: 'Stai per eliminare ' + formD.ids.length + ' clienti',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Elimina'
    }).then((result) => {
        if (result.isConfirmed) {
            formD.delete('/clients/multidel', {
            onSuccess: () => {
                // reset the array containing the just deleted ids
                formD.reset();
            },
        });
        }
    });
}

// Popup alert to confirm the client deletion
function deleteClient(c) {
    swal.fire({
        title: 'Stai per eliminare',
        text: c.name,
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Elimina'
    }).then((result) => {
        if (result.isConfirmed) {
            // Route to clients deletion
            Inertia.delete('clients/' + c.id);
        }
    });
}

// Populate the modal form with the selected Client's data
function editClient(c) {
    new_entry = false;
    formCU.name = c.name;
    formCU.address = c.address;
    formCU.email = c.email;
    formCU.phone = c.phone;
    formCU.id = c.id;
}

// Resets the modal form if a new Client must be created
function newClient() {
    new_entry = true;
    formCU.name = '';
    formCU.address = '';
    formCU.email = '';
    formCU.phone = '';
    formCU.id = 0;
}

// Sets the options to order the Clients according to the required criteria and send a request to the server
function listOrder(field) {
    if ( field_to_order != field ) {
        // Set the field name
        field_to_order = field;
        // Ordering direction to ASC
        list_order = true;
    }
    else {
        // The field has already been selected, invert the ordering direction
        list_order = ! list_order;
    }

    // Request a new list according to the ordering options
    Inertia.get('/clients', {search: search.value, field: field_to_order, order: list_order}, {
        preserveState: true,
        replace: true 
    });
}
</script>

<template>
    <Head title="Clienti" />

    <h1 class="text-center">Clienti</h1>
    <div class="d-flex justify-content-between">
        <div>
            <!-- Button trigger modal -->
            <button @click="newClient" type="button" class="btn btn-primary" data-bs-toggle="modal" data-bs-target="#clientModal">
            Nuovo cliente
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
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th scope="col"></th>
                        <th scope="col" @click="listOrder('id')" class="sortable">id
                            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-sort-down-alt" viewBox="0 0 16 16">
                                <path d="M3.5 3.5a.5.5 0 0 0-1 0v8.793l-1.146-1.147a.5.5 0 0 0-.708.708l2 1.999.007.007a.497.497 0 0 0 .7-.006l2-2a.5.5 0 0 0-.707-.708L3.5 12.293V3.5zm4 .5a.5.5 0 0 1 0-1h1a.5.5 0 0 1 0 1h-1zm0 3a.5.5 0 0 1 0-1h3a.5.5 0 0 1 0 1h-3zm0 3a.5.5 0 0 1 0-1h5a.5.5 0 0 1 0 1h-5zM7 12.5a.5.5 0 0 0 .5.5h7a.5.5 0 0 0 0-1h-7a.5.5 0 0 0-.5.5z"/>
                            </svg>
                        </th>
                        <th scope="col" @click="listOrder('name')" class="sortable">Nome 
                            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-sort-down-alt" viewBox="0 0 16 16">
                                <path d="M3.5 3.5a.5.5 0 0 0-1 0v8.793l-1.146-1.147a.5.5 0 0 0-.708.708l2 1.999.007.007a.497.497 0 0 0 .7-.006l2-2a.5.5 0 0 0-.707-.708L3.5 12.293V3.5zm4 .5a.5.5 0 0 1 0-1h1a.5.5 0 0 1 0 1h-1zm0 3a.5.5 0 0 1 0-1h3a.5.5 0 0 1 0 1h-3zm0 3a.5.5 0 0 1 0-1h5a.5.5 0 0 1 0 1h-5zM7 12.5a.5.5 0 0 0 .5.5h7a.5.5 0 0 0 0-1h-7a.5.5 0 0 0-.5.5z"/>
                            </svg>
                        </th>
                        <th scope="col">Email</th>
                        <th scope="col">Indirizzo</th>
                        <th scope="col">Telefono</th>
                        <th scope="col">Azioni</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="client in clients.data" :key="client.id">
                        <td><input type="checkbox" v-model="formD.ids" :value="client.id" /></td>
                        <td>{{ client.id }}</td>
                        <td>{{ client.name }}</td>
                        <td>{{ client.email }}</td>
                        <td>{{ client.address }}</td>
                        <td>{{ client.phone }}</td>
                        <td>
                            <div class="d-flex justify-content-between">
                                <span role="button" data-bs-toggle="modal" data-bs-target="#clientModal" @click="editClient(client)">
                                    <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-pencil-fill" viewBox="0 0 16 16">
                                        <path d="M12.854.146a.5.5 0 0 0-.707 0L10.5 1.793 14.207 5.5l1.647-1.646a.5.5 0 0 0 0-.708l-3-3zm.646 6.061L9.793 2.5 3.293 9H3.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.5h.5a.5.5 0 0 1 .5.5v.207l6.5-6.5zm-7.468 7.468A.5.5 0 0 1 6 13.5V13h-.5a.5.5 0 0 1-.5-.5V12h-.5a.5.5 0 0 1-.5-.5V11h-.5a.5.5 0 0 1-.5-.5V10h-.5a.499.499 0 0 1-.175-.032l-.179.178a.5.5 0 0 0-.11.168l-2 5a.5.5 0 0 0 .65.65l5-2a.5.5 0 0 0 .168-.11l.178-.178z"/>
                                    </svg>
                                </span>
                                <span role="button" @click="deleteClient(client)">
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

    <!-- If the size of the array links is 3 (prev, current, next) or less, all the results fit inside one page -->
    <Pagination :links="clients.links" v-if="clients.links.length > 3"/>

    <!-- Modal -->
    <div class="modal fade" id="clientModal" tabindex="-1" aria-labelledby="clientModalLabel" aria-hidden="true">
        <div class="modal-dialog modal-dialog-centered">
            <div class="modal-content">
                <div class="modal-header">
                    <h1 class="modal-title fs-5" id="clientModalLabel">Nuovo cliente</h1>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <div class="modal-body">
                    <form @submit.prevent="submitCU">
                        <div class="mb-3">
                            <label for="name" class="form-label">Nome</label>
                            <input type="name" class="form-control" v-model="formCU.name" id="name">
                            <small v-if="formCU.errors.name" v-text="formCU.errors.name" id="nameHelp" class="text-danger"></small>
                        </div>
                        <div class="mb-3">
                            <label for="address" class="form-label">Indirizzo</label>
                            <input type="address" class="form-control" v-model="formCU.address" id="address">
                            <small v-if="formCU.errors.address" v-text="formCU.errors.address" id="addressHelp" class="text-danger"></small>
                        </div>
                        <div class="mb-3">
                            <label for="email" class="form-label">Email</label>
                            <input type="email" class="form-control" v-model="formCU.email" id="email">
                            <small v-if="formCU.errors.email" v-text="formCU.errors.email" id="emailHelp" class="text-danger"></small>
                        </div>
                        <div class="mb-3">
                            <label for="phone" class="form-label">Telefono</label>
                            <input type="text" class="form-control" v-model="formCU.phone" id="phone">
                            <small v-if="formCU.errors.phone" v-text="formCU.errors.phone" id="phoneHelp" class="text-danger"></small>
                        </div>
                        <input type="hidden" v-model="formCU.id" id="clientId" />
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
