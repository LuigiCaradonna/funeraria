<?php

namespace App\Http\Controllers;

use App\Models\Cross;
use Illuminate\Http\Request;
use Inertia\Inertia;

class CrossController extends Controller
{
    /**
     * Shows the Crosses list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Crosses page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $crosses = Cross::query();

        if ( $search != "" )
        {
            $crosses->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $crosses->orderby($field, $order);
        }

        return Inertia::render('Crosses', [
            'crosses' => $crosses->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Cross
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Cross
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Crosses page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('crosses')->with('error', 'Creazione croce non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Flame
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! Cross::create($validated) )
        {
            return redirect()->route('crosses')->with('error', 'Creazione croce non riuscita.');
        }

        return redirect()->route('crosses')->with('success', 'Croce creata con successo.');
    }

    /**
     * Updates an Cross given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Cross
     * @param int $id id of the Cross to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Crosses page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('crosses')->with('error', 'Croce non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $cross = Cross::find($id);

        if ( ! $cross )
        {
            return redirect()->route('crosses')->with('error', 'Croce non trovata.');
        }

        $cross->name = $request->input('name');

        if ( ! $cross->save() )
            return redirect()->route('crosses')->with('error', 'Modifica croce non riuscita.');
            
        return redirect()->route('crosses')->with('success', 'Croce modificata con successo.');
    }

    /**
     * Deletes an Cross given its id
     * 
     * @param int $id id of the Cross to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Crosses page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('crosses')->with('error', 'Croce non trovata.');

        $cross = Cross::find($id);

        if ( ! $cross )
        {
            return redirect()->route('crosses')->with('error', 'Croce non trovata.');
        }

        if ( ! $cross->delete() )
            return redirect()->route('crosses')->with('error', 'Eliminazione croce non riuscita.');
            
        return redirect()->route('crosses')->with('success', 'Croce eliminata con successo.');
    }
    
    /**
     * Deletes a set of Crosses whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Crosses to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Crosses page
     */
    public function multiDelete(Request $request)
    {
        if ( Cross::destroy(collect($request->input('ids'))) )
            return redirect()->route('crosses')->with('success', 'Croci eliminate con successo.');

        return redirect()->route('crosses')->with('error', 'Eliminazione croci non riuscita.');
    }
}
