<?php

namespace App\Http\Controllers;

use App\Models\Outline;
use Illuminate\Http\Request;
use Inertia\Inertia;

class OutlineController extends Controller
{
    /**
     * Shows the Outlines list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Outlines page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $outlines = Outline::query();

        if ( $search != "" ) 
        {
            $outlines->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $outlines->orderby($field, $order);
        }

        return Inertia::render('Outlines', [
            'outlines' => $outlines->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Outline
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Outline
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Outlines page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('outlines')->with('error', 'Creazione cornice non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Outline::create($validated) )
        {
            return redirect()->route('outlines')->with('error', 'Creazione cornice non riuscita.');
        }

        return redirect()->route('outlines')->with('success', 'Cornice creata con successo.');
    }

    /**
     * Updates a Outline given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Outline
     * @param int $id id of the Outline to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Outlines page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('outline')->with('error', 'Righello non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $outline = Outline::find($id);

        if ( ! $outline )
        {
            return redirect()->route('outline')->with('error', 'Righello non trovato.');
        }

        $outline->name = $request->input('name');

        if ( ! $outline->save() )
            return redirect()->route('outline')->with('error', 'Modifica Righello non riuscita.');
            
        return redirect()->route('outline')->with('success', 'Righello modificato con successo.');
    }

    /**
     * Deletes a Outline given its id
     * 
     * @param int $id id of the Outline to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Outlines page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('outlines')->with('error', 'Righello non trovato.');

        // This is executed only if the validation succeedes
        $outline = Outline::find($id);

        if ( ! $outline )
        {
            return redirect()->route('outlines')->with('error', 'Righello non trovato.');
        }

        if ( ! $outline->delete() )
            return redirect()->route('outlines')->with('error', 'Eliminazione righello non riuscita.');
            
        return redirect()->route('outlines')->with('success', 'Righello eliminato con successo.');
    }
    
    /**
     * Deletes a set of Outlines whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Frame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frame page
     */
    public function multiDelete(Request $request)
    {
        if ( Outline::destroy(collect($request->input('ids'))) )
            return redirect()->route('outlines')->with('success', 'Righelli eliminati con successo.');

        return redirect()->route('outlines')->with('error', 'Eliminazione righelli non riuscita.');
    }
}
