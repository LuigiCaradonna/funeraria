<?php

namespace App\Http\Controllers;

use App\Models\Material;
use Illuminate\Http\Request;
use Inertia\Inertia;

class MaterialController extends Controller
{
    /**
     * Shows the Materials list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Materials page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $materials = Material::query();

        if ( $search != "" ) 
        {
            $materials->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $materials->orderby($field, $order);
        }

        return Inertia::render('Materials', [
            'materials' => $materials->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Material
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Material
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Materials page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('materials')->with('error', 'Creazione materiale non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Material::create($validated) )
        {
            return redirect()->route('materials')->with('error', 'Creazione materiale non riuscita.');
        }

        return redirect()->route('materials')->with('success', 'Materiale creato con successo.');
    }

    /**
     * Updates a Material given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Material
     * @param int $id id of the Material to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Materials page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('materials')->with('error', 'Materiale non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $material = Material::find($id);

        if ( ! $material )
        {
            return redirect()->route('materials')->with('error', 'Materiale non trovato.');
        }

        $material->name = $request->input('name');

        if ( ! $material->save() )
            return redirect()->route('materials')->with('error', 'Modifica materiale non riuscita.');
            
        return redirect()->route('materials')->with('success', 'Materiale modificato con successo.');
    }

    /**
     * Deletes a Material given its id
     * 
     * @param int $id id of the Material to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Materials page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('materials')->with('error', 'Materiale non trovato.');

        // This is executed only if the validation succeedes
        $material = Material::find($id);

        if ( ! $material )
        {
            return redirect()->route('materials')->with('error', 'Materiale non trovato.');
        }

        if ( ! $material->delete() )
            return redirect()->route('materials')->with('error', 'Eliminazione materiale non riuscita.');
            
        return redirect()->route('materials')->with('success', 'Materiale eliminato con successo.');
    }
    
    /**
     * Deletes a set of Materials whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Materials to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Materials page
     */
    public function multiDelete(Request $request)
    {
        if ( Material::destroy(collect($request->input('ids'))) )
            return redirect()->route('materials')->with('success', 'Materiali eliminati con successo.');

        return redirect()->route('materials')->with('error', 'Eliminazione materiali non riuscita.');
    }
}
