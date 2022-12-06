<?php

namespace App\Http\Controllers;

use App\Models\Lamp;
use Illuminate\Http\Request;
use Inertia\Inertia;

class LampController extends Controller
{
    /**
     * Shows the Lamps list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Lamps page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $lamps = Lamp::query();

        if ( $search != "" )
        {
            $lamps->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $lamps->orderby($field, $order);
        }

        return Inertia::render('Lamps', [
            'lamps' => $lamps->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Lamp
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Lamp
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Lamps page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('lamps')->with('error', 'Creazione lampada non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Lamp
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! Lamp::create($validated) )
        {
            return redirect()->route('lamps')->with('error', 'Creazione lampada non riuscita.');
        }

        return redirect()->route('lamps')->with('success', 'Lampada creata con successo.');
    }

    /**
     * Updates an Lamp given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Lamp
     * @param int $id id of the Lamp to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Lamps page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('lamps')->with('error', 'Lampada non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $lamp = Lamp::find($id);

        if ( ! $lamp )
        {
            return redirect()->route('lamps')->with('error', 'Lampada non trovata.');
        }

        $lamp->name = $request->input('name');

        if ( ! $lamp->save() )
            return redirect()->route('lamps')->with('error', 'Modifica lampada non riuscita.');
            
        return redirect()->route('lamps')->with('success', 'Lampada modificata con successo.');
    }

    /**
     * Deletes an Lamp given its id
     * 
     * @param int $id id of the Lamp to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Lamps page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('lamps')->with('error', 'Lampada non trovata.');

        $lamp = Lamp::find($id);

        if ( ! $lamp )
        {
            return redirect()->route('lamps')->with('error', 'Lampada non trovata.');
        }

        if ( ! $lamp->delete() )
            return redirect()->route('lamps')->with('error', 'Eliminazione lampada non riuscita.');
            
        return redirect()->route('lamps')->with('success', 'Lampada eliminata con successo.');
    }
    
    /**
     * Deletes a set of Lamps whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Lamps to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Lamps page
     */
    public function multiDelete(Request $request)
    {
        if ( Lamp::destroy(collect($request->input('ids'))) )
            return redirect()->route('lamps')->with('success', 'Lampade eliminate con successo.');

        return redirect()->route('lamps')->with('error', 'Eliminazione lampade non riuscita.');
    }
}
