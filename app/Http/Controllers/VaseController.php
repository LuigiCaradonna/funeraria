<?php

namespace App\Http\Controllers;

use App\Models\Vase;
use Illuminate\Http\Request;
use Inertia\Inertia;

class VaseController extends Controller
{
    /**
     * Shows the Vases list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Vases page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $vases = Vase::query();

        if ( $search != "" )
        {
            $vases->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $vases->orderby($field, $order);
        }

        return Inertia::render('Vases', [
            'vases' => $vases->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Vase
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Vase
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Vases page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('vases')->with('error', 'Creazione vaso non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Vase
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! Vase::create($validated) )
        {
            return redirect()->route('vases')->with('error', 'Creazione vaso non riuscita.');
        }

        return redirect()->route('vases')->with('success', 'Vaso creato con successo.');
    }

    /**
     * Updates an Vase given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Vase
     * @param int $id id of the Vase to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Vases page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('vases')->with('error', 'Vaso non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $vase = Vase::find($id);

        if ( ! $vase )
        {
            return redirect()->route('vases')->with('error', 'Vaso non trovato.');
        }

        $vase->name = $request->input('name');

        if ( ! $vase->save() )
            return redirect()->route('vases')->with('error', 'Modifica vaso non riuscita.');
            
        return redirect()->route('vases')->with('success', 'Vaso modificato con successo.');
    }

    /**
     * Deletes an Vase given its id
     * 
     * @param int $id id of the Vase to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Vases page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('vases')->with('error', 'Vaso non trovato.');

        $vase = Vase::find($id);

        if ( ! $vase )
        {
            return redirect()->route('vases')->with('error', 'Vaso non trovato.');
        }

        if ( ! $vase->delete() )
            return redirect()->route('vases')->with('error', 'Eliminazione vaso non riuscita.');
            
        return redirect()->route('vases')->with('success', 'Vaso eliminato con successo.');
    }
    
    /**
     * Deletes a set of Vases whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Vases to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Vases page
     */
    public function multiDelete(Request $request)
    {
        if ( Vase::destroy(collect($request->input('ids'))) )
            return redirect()->route('vases')->with('success', 'Vasi eliminati con successo.');

        return redirect()->route('vases')->with('error', 'Eliminazione vasi non riuscita.');
    }
}
