<?php

namespace App\Http\Controllers;

use App\Models\Sculpture;
use Illuminate\Http\Request;
use Inertia\Inertia;

class SculptureController extends Controller
{
    /**
     * Shows the Sculptures list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Sculptures page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $sculptures = Sculpture::query();

        if ( $search != "" ) 
        {
            $sculptures->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $sculptures->orderby($field, $order);
        }

        return Inertia::render('Sculptures', [
            'sculptures' => $sculptures->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the Sculpture page an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Sculpture
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Sculpture
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Sculptures page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('sculptures')->with('error', 'Creazione scultura non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Sculpture
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! Sculpture::create($validated) )
        {
            return redirect()->route('sculptures')->with('error', 'Creazione scultura non riuscita.');
        }

        return redirect()->route('sculptures')->with('success', 'Scultura creata con successo.');
    }

    /**
     * Updates a Sculpture given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Sculpture
     * @param int $id id of the Sculpture to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Sculptures page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('sculptures')->with('error', 'Scultura non trovata.');

        // validate() returns the validated fields on success, they can be used to create the Sculpture
        $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        $sculpture = Sculpture::find($id);

        if ( ! $sculpture )
        {
            return redirect()->route('sculptures')->with('error', 'Scultura non trovata.');
        }

        $sculpture->name = $request->input('name');

        if ( ! $sculpture->save() )
            return redirect()->route('sculptures')->with('error', 'Modifica scultura non riuscita.');
            
        return redirect()->route('sculptures')->with('success', 'Scultura modificata con successo.');
    }

    /**
     * Deletes a Sculpture given its id
     * 
     * @param int $id id of the Sculpture to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Sculptures page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('sculpltures')->with('error', 'Scultura non trovata.');

        // This is executed only if the validation succeedes
        $sculpture = Sculpture::find($id);

        if ( ! $sculpture )
        {
            return redirect()->route('sculptures')->with('error', 'Scultura non trovata.');
        }

        if ( ! $sculpture->delete() )
            return redirect()->route('sculptures')->with('error', 'Eliminazione scultura non riuscita.');
            
        return redirect()->route('sculptures')->with('success', 'Scultura eliminata con successo.');
    }
    
    /**
     * Deletes a set of Sculptures whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Sculptures to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Sculptures page
     */
    public function multiDelete(Request $request)
    {
        if ( Sculpture::destroy(collect($request->input('ids'))) )
            return redirect()->route('sculptures')->with('success', 'Scultura eliminata con successo.');

        return redirect()->route('sculptures')->with('error', 'Eliminazione scultura non riuscita.');
    }
}
