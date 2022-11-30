<?php

namespace App\Http\Controllers;

use App\Models\Font;
use Illuminate\Http\Request;
use Inertia\Inertia;

class FontController extends Controller
{
    /**
     * Shows the Fonts list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Fonts page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $fonts = Font::query();

        if ( $search != "" ) 
        {
            $fonts->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $fonts->orderby($field, $order);
        }

        return Inertia::render('Fonts', [
            'fonts' => $fonts->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Font
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Font
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Fonts page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('fonts')->with('error', 'Creazione font non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Font::create($validated) )
        {
            return redirect()->route('fonts')->with('error', 'Creazione font non riuscita.');
        }

        return redirect()->route('fonts')->with('success', 'Font creato con successo.');
    }

    /**
     * Updates a Font given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Font
     * @param int $id id of the Font to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Fonts page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('fonts')->with('error', 'Font non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $font = Font::find($id);

        if ( ! $font )
        {
            return redirect()->route('fonts')->with('error', 'Font non trovato.');
        }

        $font->name = $request->input('name');

        if ( ! $font->save() )
            return redirect()->route('fonts')->with('error', 'Modifica font non riuscita.');
            
        return redirect()->route('fonts')->with('success', 'Font modificato con successo.');
    }

    /**
     * Deletes a Font given its id
     * 
     * @param int $id id of the Font to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Fonts page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('fonts')->with('error', 'Font non trovato.');

        // This is executed only if the validation succeedes
        $font = Font::find($id);

        if ( ! $font )
        {
            return redirect()->route('fonts')->with('error', 'Font non trovato.');
        }

        if ( ! $font->delete() )
            return redirect()->route('fonts')->with('error', 'Eliminazione font non riuscita.');
            
        return redirect()->route('fonts')->with('success', 'Font eliminato con successo.');
    }
    
    /**
     * Deletes a set of Fonts whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Font to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Font page
     */
    public function multiDelete(Request $request)
    {
        if ( Font::destroy(collect($request->input('ids'))) )
            return redirect()->route('fonts')->with('success', 'Font eliminati con successo.');

        return redirect()->route('fonts')->with('error', 'Eliminazione font non riuscita.');
    }
}
