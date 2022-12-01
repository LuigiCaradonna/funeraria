<?php

namespace App\Http\Controllers;

use App\Models\TombType;
use Illuminate\Http\Request;
use Inertia\Inertia;

class TombTypeController extends Controller
{
    /**
     * Shows the TombTypes list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombTypes page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $tomb_types = TombType::query();

        if ( $search != "" )
        {
            $tomb_types->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $tomb_types->orderby($field, $order);
        }

        return Inertia::render('TombTypes', [
            'tomb_types' => $tomb_types->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new TombType
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the TombType
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombTypes page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('tomb_types')->with('error', 'Creazione tipo lapide non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the TombType
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! TombType::create($validated) )
        {
            return redirect()->route('tomb_types')->with('error', 'Creazione tipo lapide non riuscita.');
        }

        return redirect()->route('tomb_types')->with('success', 'Tipo lapide creato con successo.');
    }

    /**
     * Updates an TombType given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the TombType
     * @param int $id id of the TombType to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombTypes page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('tomb_types')->with('error', 'Tipo lapide non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $tomb_type = TombType::find($id);

        if ( ! $tomb_type )
        {
            return redirect()->route('tomb_types')->with('error', 'Tipo lapide non trovato.');
        }

        $tomb_type->name = $request->input('name');

        if ( ! $tomb_type->save() )
            return redirect()->route('tomb_types')->with('error', 'Modifica tipo lapide non riuscita.');
            
        return redirect()->route('tomb_types')->with('success', 'Tipo lapide modificato con successo.');
    }

    /**
     * Deletes an TombType given its id
     * 
     * @param int $id id of the Tomb to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombTypes page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('tomb_types')->with('error', 'Tipo lapide non trovato.');

        $tomb_type = TombType::find($id);

        if ( ! $tomb_type )
        {
            return redirect()->route('tomb_types')->with('error', 'Tipo lapide non trovato.');
        }

        if ( ! $tomb_type->delete() )
            return redirect()->route('tomb_types')->with('error', 'Eliminazione tipo lapide non riuscita.');
            
        return redirect()->route('tomb_types')->with('success', 'Tipo lapide eliminato con successo.');
    }
    
    /**
     * Deletes a set of TombTypes whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the TombTypes to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombTypes page
     */
    public function multiDelete(Request $request)
    {
        if ( TombType::destroy(collect($request->input('ids'))) )
            return redirect()->route('tomb_types')->with('success', 'Tipi lapidi eliminati con successo.');

        return redirect()->route('tomb_types')->with('error', 'Eliminazione tipi lapidi non riuscita.');
    }
}
