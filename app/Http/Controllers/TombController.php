<?php

namespace App\Http\Controllers;

use App\Models\Tomb;
use App\Models\TombType;
use Illuminate\Http\Request;
use Inertia\Inertia;

class TombController extends Controller
{
    /**
     * Shows the Tombs list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Tombs page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $tombs = Tomb::query();

        if ( $search != "" )
        {
            $tombs->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $tombs->with('tomb_type')->orderby($field, $order);
        }

        return Inertia::render('Tombs', [
            'tombs' => $tombs->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'tombtypes' => TombType::all(),
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Tomb
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Tomb
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Tombs page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('tombs')->with('error', 'Creazione lapide non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Tomb
        $validated = $request->validate([
            'name' => 'required',
            'tomb_type_id' => 'required|exists:tomb_types,id',
        ]);

        // This is executed only if the validation succeedes
        if ( ! Tomb::create($validated) )
        {
            return redirect()->route('tombs')->with('error', 'Creazione lapide non riuscita.');
        }

        return redirect()->route('tombs')->with('success', 'Lapide creata con successo.');
    }

    /**
     * Updates an Tomb given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Tomb
     * @param int $id id of the Tomb to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Tombs page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('tombs')->with('error', 'Lapide non trovata.');

        $request->validate([
            'name' => 'required',
            'tomb_type_id' => 'required|exists:tomb_types,id',
        ]);

        $tomb = Tomb::find($id);

        if ( ! $tomb )
        {
            return redirect()->route('tombs')->with('error', 'Lapide non trovata.');
        }

        $tomb->name = $request->input('name');
        $tomb->tomb_type_id = $request->input('tomb_type_id');

        if ( ! $tomb->save() )
            return redirect()->route('tombs')->with('error', 'Modifica lapide non riuscita.');
            
        return redirect()->route('tombs')->with('success', 'Lapide modificata con successo.');
    }

    /**
     * Deletes a Tomb given its id
     * 
     * @param int $id id of the Tomb to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Tombs page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('tombs')->with('error', 'Lapide non trovata.');

        $tomb = Tomb::find($id);

        if ( ! $tomb )
        {
            return redirect()->route('tombs')->with('error', 'Lapide non trovata.');
        }

        if ( ! $tomb->delete() )
            return redirect()->route('tombs')->with('error', 'Eliminazione lapide non riuscita.');
            
        return redirect()->route('tombs')->with('success', 'Lapide eliminata con successo.');
    }
    
    /**
     * Deletes a set of Tombs whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Tombs to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Tombs page
     */
    public function multiDelete(Request $request)
    {
        if ( Tomb::destroy(collect($request->input('ids'))) )
            return redirect()->route('tombs')->with('success', 'Lapidi eliminate con successo.');

        return redirect()->route('tombs')->with('error', 'Eliminazione lapidi non riuscita.');
    }
}
