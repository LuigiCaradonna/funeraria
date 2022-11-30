<?php

namespace App\Http\Controllers;

use App\Models\Color;
use Illuminate\Http\Request;
use Inertia\Inertia;

class ColorController extends Controller
{
    /**
     * Shows the Color list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Color page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $colors = Color::query();

        if ( $search != "" ) 
        {
            $colors->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $colors->orderby($field, $order);
        }

        return Inertia::render('Colors', [
            'colors' => $colors->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Color
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Color
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Colors page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('colors')->with('error', 'Creazione colore non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Color::create($validated) )
        {
            return redirect()->route('colors')->with('error', 'Creazione colore non riuscita.');
        }

        return redirect()->route('colors')->with('success', 'Colore creato con successo.');
    }

    /**
     * Updates a Color given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Color
     * @param int $id id of the Color to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Colors page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('colors')->with('error', 'Colore non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $color = Color::find($id);

        if ( ! $color )
        {
            return redirect()->route('colors')->with('error', 'Colore non trovato.');
        }

        $color->name = $request->input('name');

        if ( ! $color->save() )
            return redirect()->route('colors')->with('error', 'Modifica colore non riuscita.');
            
        return redirect()->route('colors')->with('success', 'Colore modificato con successo.');
    }

    /**
     * Deletes a Color given its id
     * 
     * @param int $id id of the Color to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Colors page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('colors')->with('error', 'Colore non trovato.');

        // This is executed only if the validation succeedes
        $color = Color::find($id);

        if ( ! $color )
        {
            return redirect()->route('colors')->with('error', 'Colore non trovato.');
        }

        if ( ! $color->delete() )
            return redirect()->route('colors')->with('error', 'Eliminazione colore non riuscita.');
            
        return redirect()->route('colors')->with('success', 'Colore eliminato con successo.');
    }
    
    /**
     * Deletes a set of Colors whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Color to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Color page
     */
    public function multiDelete(Request $request)
    {
        if ( Color::destroy(collect($request->input('ids'))) )
            return redirect()->route('colors')->with('success', 'Colori eliminati con successo.');

        return redirect()->route('colors')->with('error', 'Eliminazione colori non riuscita.');
    }
}
