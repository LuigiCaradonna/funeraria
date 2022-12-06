<?php

// APP_KEY=base64:GYL6LxbwsCT9EthPNxbKXBMIVowzRGUb77D+p4/7k14=

namespace App\Http\Controllers;

use App\Models\Client;
use Illuminate\Http\Request;
use Inertia\Inertia;

class ClientController extends Controller
{
    /**
     * Shows the Clients list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Clients page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $clients = Client::query();

        if ( $search != "" ) 
        {
            $clients->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $clients->orderby($field, $order);
        }

        return Inertia::render('Clients', [
            'clients' => $clients->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the Client page an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Client
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Client
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Clients page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('clients')->with('error', 'Creazione cliente non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Client
        $validated = $request->validate([
            'name' => 'required',
            'address' => 'nullable',
            'email' => ['nullable', 'email'],
            'phone' => ['nullable', 'regex:/^\+?[\d\s]+$/'],
        ]);

        // This is executed only if the validation succeedes
        if ( ! Client::create($validated) )
        {
            return redirect()->route('clients')->with('error', 'Creazione cliente non riuscita.');
        }

        return redirect()->route('clients')->with('success', 'Cliente creato con successo.');
    }

    /**
     * Updates a Client given his id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Client
     * @param int $id id of the Client to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Clients page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('clients')->with('error', 'Cliente non trovato.');

        // validate() returns the validated fields on success, they can be used to create the Client
        $request->validate([
            'name' => 'required',
            'address' => 'nullable',
            'email' => ['nullable', 'email'],
            'phone' => ['nullable', 'regex:/^\+?[\d\s]+$/'],
        ]);

        // This is executed only if the validation succeedes
        $client = Client::find($id);

        if ( ! $client )
        {
            return redirect()->route('clients')->with('error', 'Cliente non trovato.');
        }

        $client->name = $request->input('name');
        $client->address = $request->input('address');
        $client->email = $request->input('email');
        $client->phone = $request->input('phone');

        if ( ! $client->save() )
            return redirect()->route('clients')->with('error', 'Modifica cliente non riuscita.');
            
        return redirect()->route('clients')->with('success', 'Cliente modificato con successo.');
    }

    /**
     * Deletes a Client given his id
     * 
     * @param int $id id of the Client to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Clients page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('clients')->with('error', 'Cliente non trovato.');

        // This is executed only if the validation succeedes
        $client = Client::find($id);

        if ( ! $client )
        {
            return redirect()->route('clients')->with('error', 'Cliente non trovato.');
        }

        if ( ! $client->delete() )
            return redirect()->route('clients')->with('error', 'Eliminazione cliente non riuscita.');
            
        return redirect()->route('clients')->with('success', 'Cliente eliminato con successo.');
    }
    
    /**
     * Deletes a set of Clients whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Clients to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Clients page
     */
    public function multiDelete(Request $request)
    {
        if ( Client::destroy(collect($request->input('ids'))) )
            return redirect()->route('clients')->with('success', 'Clienti eliminati con successo.');

        return redirect()->route('clients')->with('error', 'Eliminazione clienti non riuscita.');
    }
}
