<?php

namespace App\Http\Controllers;

use App\Models\Picture;
use Illuminate\Http\Request;
use Inertia\Inertia;

class PictureController extends Controller
{
    /**
     * Shows the Pictures list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Pictures page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $pictures = Picture::query();

        if ( $search != "" ) 
        {
            $pictures->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $pictures->orderby($field, $order);
        }

        return Inertia::render('Pictures', [
            'pictures' => $pictures->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Pictures
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Pictures
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Picturess page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('pictures')->with('error', 'Creazione foto non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Picture::create($validated) )
        {
            return redirect()->route('pictures')->with('error', 'Creazione foto non riuscita.');
        }

        return redirect()->route('pictures')->with('success', 'Foto creato con successo.');
    }

    /**
     * Updates a Pictures given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Pictures
     * @param int $id id of the Pictures to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Pictures page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('pictures')->with('error', 'Foto non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $picture = Picture::find($id);

        if ( ! $picture )
        {
            return redirect()->route('pictures')->with('error', 'Foto non trovata.');
        }

        $picture->name = $request->input('name');

        if ( ! $picture->save() )
            return redirect()->route('pictures')->with('error', 'Modifica foto non riuscita.');
            
        return redirect()->route('pictures')->with('success', 'Foto modificata con successo.');
    }

    /**
     * Deletes a Pictures given its id
     * 
     * @param int $id id of the Pictures to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Pictures page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('pictures')->with('error', 'Foto non trovata.');

        // This is executed only if the validation succeedes
        $picture = Picture::find($id);

        if ( ! $picture )
        {
            return redirect()->route('pictures')->with('error', 'Foto non trovata.');
        }

        if ( ! $picture->delete() )
            return redirect()->route('pictures')->with('error', 'Eliminazione foto non riuscita.');
            
        return redirect()->route('pictures')->with('success', 'Foto eliminata con successo.');
    }
    
    /**
     * Deletes a set of Pictures whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Pictures to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Pictures page
     */
    public function multiDelete(Request $request)
    {
        if ( Picture::destroy(collect($request->input('ids'))) )
            return redirect()->route('pictures')->with('success', 'Foto eliminate con successo.');

        return redirect()->route('pictures')->with('error', 'Eliminazione foto non riuscita.');
    }
}
